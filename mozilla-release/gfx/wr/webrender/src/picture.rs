<<<<<<< HEAD
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use api::{DeviceRect, FilterOp, MixBlendMode, PipelineId, PremultipliedColorF, PictureRect, PicturePoint};
use api::{DeviceIntRect, DevicePoint, LayoutRect, PictureToRasterTransform, LayoutPixel, PropertyBinding, PropertyBindingId};
use api::{DevicePixelScale, RasterRect, RasterSpace, PictureSize, DeviceIntPoint, ColorF, ImageKey, DirtyRect};
use api::{PicturePixel, RasterPixel, WorldPixel, WorldRect, ImageFormat, ImageDescriptor};
use box_shadow::{BLUR_SAMPLE_SCALE};
use clip::{ClipNodeCollector, ClipStore, ClipChainId, ClipChainNode};
use clip_scroll_tree::{ROOT_SPATIAL_NODE_INDEX, ClipScrollTree, SpatialNodeIndex};
use device::TextureFilter;
use euclid::{TypedScale, vec3, TypedRect, TypedPoint2D, TypedSize2D};
use euclid::approxeq::ApproxEq;
use intern::ItemUid;
use internal_types::{FastHashMap, PlaneSplitter};
use frame_builder::{FrameBuildingContext, FrameBuildingState, PictureState, PictureContext};
use gpu_cache::{GpuCache, GpuCacheAddress, GpuCacheHandle};
use gpu_types::{TransformPalette, TransformPaletteId, UvRectKind};
use internal_types::FastHashSet;
use plane_split::{Clipper, Polygon, Splitter};
use prim_store::{PictureIndex, PrimitiveInstance, SpaceMapper, VisibleFace, PrimitiveInstanceKind};
use prim_store::{get_raster_rects, CoordinateSpaceMapping};
use prim_store::{OpacityBindingStorage, PrimitiveTemplateKind, ImageInstanceStorage, OpacityBindingIndex, SizeKey};
use render_backend::FrameResources;
use render_task::{ClearMode, RenderTask, RenderTaskCacheEntryHandle, TileBlit};
use render_task::{RenderTaskCacheKey, RenderTaskCacheKeyKind, RenderTaskId, RenderTaskLocation};
use resource_cache::ResourceCache;
use scene::{FilterOpHelpers, SceneProperties};
use scene_builder::DocumentResources;
use smallvec::SmallVec;
use surface::{SurfaceDescriptor, TransformKey};
use std::{mem, ops};
use texture_cache::{Eviction, TextureCacheHandle};
use tiling::RenderTargetKind;
use util::{TransformedRectKind, MatrixHelpers, MaxRect, RectHelpers};

/*
 A picture represents a dynamically rendered image. It consists of:

 * A number of primitives that are drawn onto the picture.
 * A composite operation describing how to composite this
   picture into its parent.
 * A configuration describing how to draw the primitives on
   this picture (e.g. in screen space or local space).
 */

/// Information about a picture that is pushed / popped on the
/// PictureUpdateState during picture traversal pass.
struct PictureInfo {
    /// The spatial node for this picture.
    spatial_node_index: SpatialNodeIndex,
}

/// Unit for tile coordinates.
#[derive(Hash, Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct TileCoordinate;

// Geometry types for tile coordinates.
pub type TileOffset = TypedPoint2D<i32, TileCoordinate>;
pub type TileSize = TypedSize2D<i32, TileCoordinate>;
pub type TileRect = TypedRect<i32, TileCoordinate>;

/// The size in device pixels of a cached tile. The currently chosen
/// size is arbitrary. We should do some profiling to find the best
/// size for real world pages.
pub const TILE_SIZE_DP: i32 = 512;

/// Information about the state of a transform dependency.
#[derive(Debug)]
pub struct TileTransformInfo {
    /// The spatial node in the current clip-scroll tree that
    /// this transform maps to.
    spatial_node_index: SpatialNodeIndex,
    /// Tiles check this to see if the dependencies have changed.
    changed: bool,
}

#[derive(Debug)]
pub struct GlobalTransformInfo {
    /// Current (quantized) value of the transform, that is
    /// independent of the value of the spatial node index.
    /// Only calculated on first use.
    current: Option<TransformKey>,
    /// Tiles check this to see if the dependencies have changed.
    changed: bool,
}

/// Information about the state of an opacity binding.
#[derive(Debug)]
pub struct OpacityBindingInfo {
    /// The current value retrieved from dynamic scene properties.
    value: f32,
    /// True if it was changed (or is new) since the last frame build.
    changed: bool,
}

/// Information about a cached tile.
#[derive(Debug)]
pub struct Tile {
    /// The set of opacity bindings that this tile depends on.
    opacity_bindings: FastHashSet<PropertyBindingId>,
    /// Set of image keys that this tile depends on.
    image_keys: FastHashSet<ImageKey>,
    /// If true, this tile is marked valid, and the existing texture
    /// cache handle can be used. Tiles are invalidated during the
    /// build_dirty_regions method.
    is_valid: bool,
    /// If false, this tile cannot be cached (e.g. it has an external
    /// video image attached to it). In future, we could add an API
    /// for the embedder to tell us if the external image changed.
    /// This is set during primitive dependency updating.
    is_cacheable: bool,
    /// If true, this tile is currently in use by the cache. It
    /// may be false if the tile is outside the bounding rect of
    /// the current picture, but hasn't been discarded yet. This
    /// is calculated during primitive dependency updating.
    pub in_use: bool,
    /// If true, this tile is currently visible on screen. This
    /// is calculated during build_dirty_regions.
    pub is_visible: bool,
    /// Handle to the cached texture for this tile.
    pub handle: TextureCacheHandle,
    /// A map from clip-scroll tree spatial node indices to the tile
    /// transforms. This allows the tile transforms to be stable
    /// if the content of the tile is the same, but the shape of the
    /// clip-scroll tree changes between scenes in other areas.
    tile_transform_map: FastHashMap<SpatialNodeIndex, TileTransformIndex>,
    /// Information about the transforms that is not part of the cache key.
    transform_info: Vec<TileTransformInfo>,
    /// Uniquely describes the content of this tile, in a way that can be
    /// (reasonably) efficiently hashed and compared.
    descriptor: TileDescriptor,
}

impl Tile {
    /// Construct a new, invalid tile.
    fn new(
        tile_offset: TileOffset,
        local_tile_size: SizeKey,
        raster_transform: TransformKey,
    ) -> Self {
        Tile {
            opacity_bindings: FastHashSet::default(),
            image_keys: FastHashSet::default(),
            is_valid: false,
            is_visible: false,
            is_cacheable: true,
            in_use: false,
            handle: TextureCacheHandle::invalid(),
            descriptor: TileDescriptor::new(
                tile_offset,
                local_tile_size,
                raster_transform,
            ),
            tile_transform_map: FastHashMap::default(),
            transform_info: Vec::new(),
        }
    }

    /// Add a (possibly) new transform dependency to this tile.
    fn push_transform_dependency(
        &mut self,
        spatial_node_index: SpatialNodeIndex,
        surface_spatial_node_index: SpatialNodeIndex,
        clip_scroll_tree: &ClipScrollTree,
        global_transforms: &mut [GlobalTransformInfo],
    ) {
        // If the primitive is positioned by the same spatial
        // node as the surface, we don't care about it since
        // the primitive can never move to a different position
        // relative to the surface.
        if spatial_node_index == surface_spatial_node_index {
            return;
        }

        let transform_info = &mut self.transform_info;
        let descriptor = &mut self.descriptor;

        // Get the mapping from unstable spatial node index to
        // a local transform index within this tile.
        let tile_transform_index = self
            .tile_transform_map
            .entry(spatial_node_index)
            .or_insert_with(|| {
                let index = transform_info.len();

                let mapping: CoordinateSpaceMapping<LayoutPixel, PicturePixel> = CoordinateSpaceMapping::new(
                    surface_spatial_node_index,
                    spatial_node_index,
                    clip_scroll_tree,
                ).expect("todo: handle invalid mappings");

                // See if the transform changed, and cache the current
                // transform if not set before.
                let changed = get_global_transform_changed(
                    global_transforms,
                    spatial_node_index,
                    clip_scroll_tree,
                    surface_spatial_node_index,
                );

                transform_info.push(TileTransformInfo {
                    changed,
                    spatial_node_index,
                });

                let key = mapping.into();

                descriptor.transforms.push(key);

                TileTransformIndex(index as u32)
            });

        // Record the transform for this primitive / clip node.
        // TODO(gw): It might be worth storing these in runs, since they
        //           probably don't change very often between prims.
        descriptor.transform_ids.push(*tile_transform_index);
    }

    /// Destroy a tile, optionally returning a handle and cache descriptor,
    /// if this surface was valid and may be useful on the next scene.
    fn destroy(self) -> Option<(TileDescriptor, TextureCacheHandle)> {
        if self.is_valid {
            Some((self.descriptor, self.handle))
        } else {
            None
        }
    }
}

/// Index of a transform array local to the tile.
#[derive(Debug, Copy, Clone, Eq, PartialEq, Hash)]
pub struct TileTransformIndex(u32);

/// Uniquely describes the content of this tile, in a way that can be
/// (reasonably) efficiently hashed and compared.
#[derive(Debug, Eq, PartialEq, Hash)]
pub struct TileDescriptor {
    /// List of primitive unique identifiers. The uid is guaranteed
    /// to uniquely describe the content of the primitive.
    pub prim_uids: Vec<ItemUid>,

    /// List of clip node unique identifiers. The uid is guaranteed
    /// to uniquely describe the content of the clip node.
    pub clip_uids: Vec<ItemUid>,

    /// List of local tile transform ids that are used to position
    /// the primitive and clip items above.
    pub transform_ids: Vec<TileTransformIndex>,

    /// List of transforms used by this tile, along with the current
    /// quantized value.
    pub transforms: Vec<TransformKey>,

    /// The set of opacity bindings that this tile depends on.
    // TODO(gw): Ugh, get rid of all opacity binding support!
    pub opacity_bindings: Vec<PropertyBindingId>,

    /// Ensures that we hash to a tile in the same local position.
    pub tile_offset: TileOffset,
    pub local_tile_size: SizeKey,

    /// Identifies the raster configuration of the rasterization
    /// root, to ensure tiles are invalidated if they are drawn in
    /// screen-space with an incompatible transform.
    pub raster_transform: TransformKey,
}

impl TileDescriptor {
    fn new(
        tile_offset: TileOffset,
        local_tile_size: SizeKey,
        raster_transform: TransformKey,
    ) -> Self {
        TileDescriptor {
            prim_uids: Vec::new(),
            clip_uids: Vec::new(),
            transform_ids: Vec::new(),
            opacity_bindings: Vec::new(),
            transforms: Vec::new(),
            tile_offset,
            raster_transform,
            local_tile_size,
        }
    }

    /// Clear the dependency information for a tile, when the dependencies
    /// are being rebuilt.
    fn clear(&mut self) {
        self.prim_uids.clear();
        self.clip_uids.clear();
        self.transform_ids.clear();
        self.transforms.clear();
        self.opacity_bindings.clear();
    }
}

/// Represents the dirty region of a tile cache picture.
/// In future, we will want to support multiple dirty
/// regions.
#[derive(Debug)]
pub struct DirtyRegion {
    tile_offset: DeviceIntPoint,
    dirty_rect: PictureRect,
    dirty_world_rect: WorldRect,
}

/// Represents a cache of tiles that make up a picture primitives.
pub struct TileCache {
    /// The size of each tile in local-space coordinates of the picture.
    pub local_tile_size: PictureSize,
    /// List of tiles present in this picture (stored as a 2D array)
    pub tiles: Vec<Tile>,
    /// A set of tiles that were used last time we built
    /// the tile grid, that may be reused or discarded next time.
    pub old_tiles: FastHashMap<TileOffset, Tile>,
    /// The current size of the rect in tile coordinates.
    pub tile_rect: TileRect,
    /// List of transform keys - used to check if transforms
    /// have changed.
    pub transforms: Vec<GlobalTransformInfo>,
    /// List of opacity bindings, with some extra information
    /// about whether they changed since last frame.
    pub opacity_bindings: FastHashMap<PropertyBindingId, OpacityBindingInfo>,
    /// A helper struct to map local rects into picture coords.
    pub space_mapper: SpaceMapper<LayoutPixel, PicturePixel>,
    /// If true, we need to update the prim dependencies, due
    /// to relative transforms changing. The dependencies are
    /// stored in each tile, and are a list of things that
    /// force the tile to re-rasterize if they change (e.g.
    /// images, transforms).
    pub needs_update: bool,
    /// If Some(..) the region that is dirty in this picture.
    pub dirty_region: Option<DirtyRegion>,
    /// The current transform of the surface itself, to allow
    /// invalidating tiles if the surface transform changes.
    /// This is only relevant when raster_space == RasterSpace::Screen.
    raster_transform: TransformKey,
}

impl TileCache {
    /// Construct a new tile cache.
    pub fn new() -> Self {
        TileCache {
            tiles: Vec::new(),
            old_tiles: FastHashMap::default(),
            tile_rect: TileRect::zero(),
            local_tile_size: PictureSize::zero(),
            transforms: Vec::new(),
            opacity_bindings: FastHashMap::default(),
            needs_update: true,
            dirty_region: None,
            space_mapper: SpaceMapper::new(
                ROOT_SPATIAL_NODE_INDEX,
                PictureRect::zero(),
            ),
            raster_transform: TransformKey::Local,
        }
    }

    /// Update the transforms array for this tile cache from the clip-scroll
    /// tree. This marks each transform as changed for later use during
    /// tile invalidation.
    pub fn update_transforms(
        &mut self,
        surface_spatial_node_index: SpatialNodeIndex,
        raster_spatial_node_index: SpatialNodeIndex,
        raster_space: RasterSpace,
        frame_context: &FrameBuildingContext,
    ) {
        // Initialize the space mapper with current bounds,
        // which is used during primitive dependency updates.
        let world_mapper = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            frame_context.screen_world_rect,
            frame_context.clip_scroll_tree,
        );

        let pic_bounds = world_mapper
            .unmap(&frame_context.screen_world_rect)
            .unwrap_or(PictureRect::max_rect());

        self.space_mapper = SpaceMapper::new(
            surface_spatial_node_index,
            pic_bounds,
        );

        // Work out the local space size of each tile, based on the
        // device pixel size of tiles.
        // TODO(gw): Perhaps add a map_point API to SpaceMapper.
        let world_tile_rect = WorldRect::from_floats(
            0.0,
            0.0,
            TILE_SIZE_DP as f32 / frame_context.device_pixel_scale.0,
            TILE_SIZE_DP as f32 / frame_context.device_pixel_scale.0,
        );
        let local_tile_rect = world_mapper
            .unmap(&world_tile_rect)
            .expect("bug: unable to get local tile size");
        self.local_tile_size = local_tile_rect.size;

        // Walk the transforms and see if we need to rebuild the primitive
        // dependencies for each tile.
        // TODO(gw): We could be smarter here and only rebuild for the primitives
        //           which are affected by transforms that have changed.
        if self.transforms.len() == frame_context.clip_scroll_tree.spatial_nodes.len() {
            for (i, transform) in self.transforms.iter_mut().enumerate() {
                // If this relative transform was used on the previous frame,
                // update it and store whether it changed for use during
                // tile invalidation later.
                if let Some(ref mut current) = transform.current {
                    let mapping: CoordinateSpaceMapping<LayoutPixel, PicturePixel> = CoordinateSpaceMapping::new(
                        surface_spatial_node_index,
                        SpatialNodeIndex(i),
                        frame_context.clip_scroll_tree,
                    ).expect("todo: handle invalid mappings");

                    let key = mapping.into();
                    transform.changed = key != *current;
                    *current = key;
                }
            }
        } else {
            // If the size of the transforms array changed, just invalidate all the transforms for now.
            self.transforms.clear();

            for _ in 0 .. frame_context.clip_scroll_tree.spatial_nodes.len() {
                self.transforms.push(GlobalTransformInfo {
                    current: None,
                    changed: true,
                });
            }
        };

        // Do a hacky diff of opacity binding values from the last frame. This is
        // used later on during tile invalidation tests.
        let current_properties = frame_context.scene_properties.float_properties();
        let old_properties = mem::replace(&mut self.opacity_bindings, FastHashMap::default());
        for (id, value) in current_properties {
            let changed = match old_properties.get(id) {
                Some(old_property) => !old_property.value.approx_eq(value),
                None => true,
            };
            self.opacity_bindings.insert(*id, OpacityBindingInfo {
                value: *value,
                changed,
            });
        }

        // Update the state of the transform for compositing this picture.
        self.raster_transform = match raster_space {
            RasterSpace::Screen => {
                // In general cases, if we're rasterizing a picture in screen space, then the
                // value of the surface spatial node will affect the contents of the picture
                // itself. However, if the surface and raster spatial nodes are in the same
                // coordinate system (which is the common case!) then we are effectively drawing
                // in a local space anyway, so don't care about that transform for the purposes
                // of validating the surface cache contents.

                let mut key = CoordinateSpaceMapping::<LayoutPixel, PicturePixel>::new(
                    raster_spatial_node_index,
                    surface_spatial_node_index,
                    frame_context.clip_scroll_tree,
                ).expect("bug: unable to get coord mapping").into();

                if let TransformKey::ScaleOffset(ref mut key) = key {
                    key.offset_x = 0.0;
                    key.offset_y = 0.0;
                }

                key
            }
            RasterSpace::Local(..) => {
                TransformKey::local()
            }
        };

        // Walk the transforms and see if we need to rebuild the primitive
        // dependencies for each tile.
        // TODO(gw): We could be smarter here and only rebuild for the primitives
        //           which are affected by transforms that have changed.
        for tile in &mut self.tiles {
            tile.descriptor.local_tile_size = self.local_tile_size.into();
            tile.descriptor.raster_transform = self.raster_transform.clone();

            debug_assert_eq!(tile.transform_info.len(), tile.descriptor.transforms.len());
            for (info, transform) in tile.transform_info.iter_mut().zip(tile.descriptor.transforms.iter_mut()) {
                let mapping: CoordinateSpaceMapping<LayoutPixel, PicturePixel> = CoordinateSpaceMapping::new(
                    surface_spatial_node_index,
                    info.spatial_node_index,
                    frame_context.clip_scroll_tree,
                ).expect("todo: handle invalid mappings");
                let new_transform = mapping.into();

                info.changed = *transform != new_transform;
                *transform = new_transform;

                self.needs_update |= info.changed;
            }
        }

        // If we need to update the dependencies for tiles, walk each tile
        // and clear the transforms and opacity bindings arrays.
        if self.needs_update {
            debug_assert!(self.old_tiles.is_empty());

            // Clear any dependencies on the set of existing tiles, since
            // they are going to be rebuilt. Drain the tiles list and add
            // them to the old_tiles hash, for re-use next frame build.
            for (i, mut tile) in self.tiles.drain(..).enumerate() {
                let y = i as i32 / self.tile_rect.size.width;
                let x = i as i32 % self.tile_rect.size.width;
                tile.descriptor.clear();
                tile.transform_info.clear();
                tile.tile_transform_map.clear();
                tile.opacity_bindings.clear();
                tile.image_keys.clear();
                tile.in_use = false;
                let key = TileOffset::new(
                    x + self.tile_rect.origin.x,
                    y + self.tile_rect.origin.y,
                );
                self.old_tiles.insert(key, tile);
            }

            // Reset the size of the tile grid.
            self.tile_rect = TileRect::zero();
        }
    }

    /// Resize the 2D tiles array if needed in order to fit dependencies
    /// for a given primitive.
    fn reconfigure_tiles_if_required(
        &mut self,
        mut x0: i32,
        mut y0: i32,
        mut x1: i32,
        mut y1: i32,
    ) {
        // Determine and store the tile bounds that are now required.
        if self.tile_rect.size.width > 0 {
            x0 = x0.min(self.tile_rect.origin.x);
            x1 = x1.max(self.tile_rect.origin.x + self.tile_rect.size.width);
        }
        if self.tile_rect.size.height > 0 {
            y0 = y0.min(self.tile_rect.origin.y);
            y1 = y1.max(self.tile_rect.origin.y + self.tile_rect.size.height);
        }

        // See how many tiles are now required, and if that's different from current config.
        let x_tiles = x1 - x0;
        let y_tiles = y1 - y0;

        if self.tile_rect.size.width == x_tiles && self.tile_rect.size.height == y_tiles {
            return;
        }

        // We will need to create a new tile array.
        let mut new_tiles = Vec::with_capacity((x_tiles * y_tiles) as usize);

        for y in 0 .. y_tiles {
            for x in 0 .. x_tiles {
                // See if we can re-use an existing tile from the old array, by mapping
                // the tile address. This saves invalidating existing tiles when we
                // just resize the picture by adding / remove primitives.
                let tx = x0 - self.tile_rect.origin.x + x;
                let ty = y0 - self.tile_rect.origin.y + y;
                let tile_offset = TileOffset::new(x + x0, y + y0);

                let tile = if tx >= 0 && ty >= 0 && tx < self.tile_rect.size.width && ty < self.tile_rect.size.height {
                    let index = (ty * self.tile_rect.size.width + tx) as usize;
                    mem::replace(
                        &mut self.tiles[index],
                        Tile::new(
                            tile_offset,
                            self.local_tile_size.into(),
                            self.raster_transform.clone(),
                        )
                    )
                } else {
                    self.old_tiles.remove(&tile_offset).unwrap_or_else(|| {
                        Tile::new(
                            tile_offset,
                            self.local_tile_size.into(),
                            self.raster_transform.clone(),
                        )
                    })
                };
                new_tiles.push(tile);
            }
        }

        self.tiles = new_tiles;
        self.tile_rect.origin = TileOffset::new(x0, y0);
        self.tile_rect.size = TileSize::new(x_tiles, y_tiles);
    }

    /// Update the dependencies for each tile for a given primitive instance.
    pub fn update_prim_dependencies(
        &mut self,
        prim_instance: &PrimitiveInstance,
        surface_spatial_node_index: SpatialNodeIndex,
        clip_scroll_tree: &ClipScrollTree,
        resources: &FrameResources,
        clip_chain_nodes: &[ClipChainNode],
        pictures: &[PicturePrimitive],
        resource_cache: &ResourceCache,
        opacity_binding_store: &OpacityBindingStorage,
        image_instances: &ImageInstanceStorage,
    ) {
        self.space_mapper.set_target_spatial_node(
            prim_instance.spatial_node_index,
            clip_scroll_tree,
        );

        let prim_data = &resources.as_common_data(&prim_instance);

        let prim_rect = LayoutRect::new(
            prim_instance.prim_origin,
            prim_data.prim_size,
        );
        let clip_rect = prim_data
            .prim_relative_clip_rect
            .translate(&prim_instance.prim_origin.to_vector());

        // Map the primitive local rect into the picture space.
        // TODO(gw): We should maybe store this in the primitive template
        //           during interning so that we never have to calculate
        //           it during frame building.
        let culling_rect = match prim_rect.intersection(&clip_rect) {
            Some(rect) => rect,
            None => return,
        };

        let rect = match self.space_mapper.map(&culling_rect) {
            Some(rect) => rect,
            None => {
                return;
            }
        };

        // If the rect is invalid, no need to create dependencies.
        // TODO(gw): Need to handle pictures with filters here.
        if rect.size.width <= 0.0 || rect.size.height <= 0.0 {
            return;
        }

        // Get the tile coordinates in the picture space.
        let x0 = (rect.origin.x / self.local_tile_size.width).floor() as i32;
        let y0 = (rect.origin.y / self.local_tile_size.height).floor() as i32;
        let x1 = ((rect.origin.x + rect.size.width) / self.local_tile_size.width).ceil() as i32;
        let y1 = ((rect.origin.y + rect.size.height) / self.local_tile_size.height).ceil() as i32;

        // Update the tile array allocation if needed.
        self.reconfigure_tiles_if_required(x0, y0, x1, y1);

        // Build the list of resources that this primitive has dependencies on.
        let mut opacity_bindings: SmallVec<[PropertyBindingId; 4]> = SmallVec::new();
        let mut clip_chain_spatial_nodes: SmallVec<[SpatialNodeIndex; 8]> = SmallVec::new();
        let mut clip_chain_uids: SmallVec<[ItemUid; 8]> = SmallVec::new();
        let mut image_keys: SmallVec<[ImageKey; 8]> = SmallVec::new();
        let mut current_clip_chain_id = prim_instance.clip_chain_id;

        // Some primitives can not be cached (e.g. external video images)
        let is_cacheable = prim_instance.is_cacheable(
            &resources.prim_data_store,
            resource_cache,
        );

        match prim_instance.kind {
            PrimitiveInstanceKind::Picture { pic_index,.. } => {
                // Pictures can depend on animated opacity bindings.
                let pic = &pictures[pic_index.0];
                if let Some(PictureCompositeMode::Filter(FilterOp::Opacity(binding, _))) = pic.requested_composite_mode {
                    if let PropertyBinding::Binding(key, _) = binding {
                        opacity_bindings.push(key.id);
                    }
                }
            }
            PrimitiveInstanceKind::Rectangle { opacity_binding_index, .. } => {
                if opacity_binding_index != OpacityBindingIndex::INVALID {
                    let opacity_binding = &opacity_binding_store[opacity_binding_index];
                    for binding in &opacity_binding.bindings {
                        if let PropertyBinding::Binding(key, _) = binding {
                            opacity_bindings.push(key.id);
                        }
                    }
                }
            }
            PrimitiveInstanceKind::Image { data_handle, image_instance_index, .. } => {
                let prim_data = &resources.prim_data_store[data_handle];
                let image_instance = &image_instances[image_instance_index];
                let opacity_binding_index = image_instance.opacity_binding_index;

                if opacity_binding_index != OpacityBindingIndex::INVALID {
                    let opacity_binding = &opacity_binding_store[opacity_binding_index];
                    for binding in &opacity_binding.bindings {
                        if let PropertyBinding::Binding(key, _) = binding {
                            opacity_bindings.push(key.id);
                        }
                    }
                }

                match prim_data.kind {
                    PrimitiveTemplateKind::Image { key, .. } => {
                        image_keys.push(key);
                    }
                    _ => {
                        unreachable!();
                    }
                }
            }
            PrimitiveInstanceKind::YuvImage { data_handle, .. } => {
                let prim_data = &resources.prim_data_store[data_handle];
                match prim_data.kind {
                    PrimitiveTemplateKind::YuvImage { ref yuv_key, .. } => {
                        image_keys.extend_from_slice(yuv_key);
                    }
                    _ => {
                        unreachable!();
                    }
                }
            }
            PrimitiveInstanceKind::TextRun { .. } |
            PrimitiveInstanceKind::LineDecoration { .. } |
            PrimitiveInstanceKind::Clear { .. } |
            PrimitiveInstanceKind::NormalBorder { .. } |
            PrimitiveInstanceKind::LinearGradient { .. } |
            PrimitiveInstanceKind::RadialGradient { .. } |
            PrimitiveInstanceKind::ImageBorder { .. } => {
                // These don't contribute dependencies
            }
        }

        // The transforms of any clips that are relative to the picture may affect
        // the content rendered by this primitive.
        while current_clip_chain_id != ClipChainId::NONE {
            let clip_chain_node = &clip_chain_nodes[current_clip_chain_id.0 as usize];
            // We only care about clip nodes that have transforms that are children
            // of the surface, since clips that are positioned by parents will be
            // handled by the clip collector when these tiles are composited.
            if clip_chain_node.spatial_node_index > surface_spatial_node_index {
                clip_chain_spatial_nodes.push(clip_chain_node.spatial_node_index);
                clip_chain_uids.push(clip_chain_node.handle.uid());
            }
            current_clip_chain_id = clip_chain_node.parent_clip_chain_id;
        }

        // Normalize the tile coordinates before adding to tile dependencies.
        // For each affected tile, mark any of the primitive dependencies.
        for y in y0 - self.tile_rect.origin.y .. y1 - self.tile_rect.origin.y {
            for x in x0 - self.tile_rect.origin.x .. x1 - self.tile_rect.origin.x {
                let index = (y * self.tile_rect.size.width + x) as usize;
                let tile = &mut self.tiles[index];

                // Mark if the tile is cacheable at all.
                tile.is_cacheable &= is_cacheable;
                tile.in_use = true;

                // Include any image keys this tile depends on.
                for image_key in &image_keys {
                    tile.image_keys.insert(*image_key);
                }

                // Include the transform of the primitive itself.
                tile.push_transform_dependency(
                    prim_instance.spatial_node_index,
                    surface_spatial_node_index,
                    clip_scroll_tree,
                    &mut self.transforms,
                );

                // Include the transforms of any relevant clip nodes for this primitive.
                for clip_chain_spatial_node in &clip_chain_spatial_nodes {
                    tile.push_transform_dependency(
                        *clip_chain_spatial_node,
                        surface_spatial_node_index,
                        clip_scroll_tree,
                        &mut self.transforms,
                    );
                }

                // Include any opacity bindings this primitive depends on.
                for id in &opacity_bindings {
                    if tile.opacity_bindings.insert(*id) {
                        tile.descriptor.opacity_bindings.push(*id);
                    }
                }

                // Update the tile descriptor, used for tile comparison during scene swaps.
                tile.descriptor.prim_uids.push(prim_instance.uid());
                tile.descriptor.clip_uids.extend_from_slice(&clip_chain_uids);
            }
        }
    }

    /// Build the dirty region(s) for the tile cache after all primitive
    /// dependencies have been updated.
    pub fn build_dirty_regions(
        &mut self,
        surface_spatial_node_index: SpatialNodeIndex,
        frame_context: &FrameBuildingContext,
        resource_cache: &mut ResourceCache,
        gpu_cache: &mut GpuCache,
        retained_tiles: &mut FastHashMap<TileDescriptor, TextureCacheHandle>,
    ) {
        self.needs_update = false;

        for (_, tile) in self.old_tiles.drain() {
            resource_cache.texture_cache.mark_unused(&tile.handle);
        }

        let world_mapper = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            frame_context.screen_world_rect,
            frame_context.clip_scroll_tree,
        );

        let mut tile_offset = DeviceIntPoint::new(
            self.tile_rect.size.width,
            self.tile_rect.size.height,
        );

        let mut dirty_rect = PictureRect::zero();

        // Step through each tile and invalidate if the dependencies have changed.
        for y in 0 .. self.tile_rect.size.height {
            for x in 0 .. self.tile_rect.size.width {
                let i = y * self.tile_rect.size.width + x;
                let tile = &mut self.tiles[i as usize];

                // If this tile is unused (has no primitives on it), we can just
                // skip any invalidation / dirty region work for it.
                if !tile.in_use {
                    continue;
                }

                let tile_rect = PictureRect::new(
                    PicturePoint::new(
                        (self.tile_rect.origin.x + x) as f32 * self.local_tile_size.width,
                        (self.tile_rect.origin.y + y) as f32 * self.local_tile_size.height,
                    ),
                    self.local_tile_size,
                );

                // Check if this tile is actually visible.
                let tile_world_rect = world_mapper
                    .map(&tile_rect)
                    .expect("bug: unable to map tile to world coords");
                tile.is_visible = frame_context.screen_world_rect.intersects(&tile_world_rect);

                // Try to reuse cached tiles from the previous scene in this new
                // scene, if possible.
                if tile.is_visible && !resource_cache.texture_cache.is_allocated(&tile.handle) {
                    // See if we have a retained tile from last scene that matches the
                    // exact content of this tile.
                    if let Some(retained_handle) = retained_tiles.remove(&tile.descriptor) {
                        // Only use if not evicted from texture cache in the meantime.
                        if resource_cache.texture_cache.is_allocated(&retained_handle) {
                            // We found a matching tile from the previous scene, so use it!
                            tile.handle = retained_handle;
                            tile.is_valid = true;
                            // We know that the hash key of the descriptor validates that
                            // the local transforms in this tile exactly match the value
                            // of the current relative transforms needed for this tile,
                            // so we can mark those transforms as valid to avoid the
                            // retained tile being invalidated below.
                            for info in &mut tile.transform_info {
                                info.changed = false;
                            }
                        }
                    }
                }

                // Invalidate the tile if not cacheable
                if !tile.is_cacheable {
                    tile.is_valid = false;
                }

                // Invalidate the tile if any images have changed
                for image_key in &tile.image_keys {
                    if resource_cache.is_image_dirty(*image_key) {
                        tile.is_valid = false;
                        break;
                    }
                }

                // Invalidate the tile if any opacity bindings changed.
                for id in &tile.opacity_bindings {
                    let changed = match self.opacity_bindings.get(id) {
                        Some(info) => info.changed,
                        None => true,
                    };
                    if changed {
                        tile.is_valid = false;
                        break;
                    }
                }

                // Invalidate the tile if any dependent transforms changed
                for info in &tile.transform_info {
                    if info.changed {
                        tile.is_valid = false;
                        break;
                    }
                }

                // Invalidate the tile if it was evicted by the texture cache.
                if !resource_cache.texture_cache.is_allocated(&tile.handle) {
                    tile.is_valid = false;
                }

                if tile.is_visible {
                    // Ensure we request the texture cache handle for this tile
                    // each frame it will be used so the texture cache doesn't
                    // decide to evict tiles that we currently want to use.
                    resource_cache.texture_cache.request(&tile.handle, gpu_cache);

                    // If we have an invalid tile, which is also visible, add it to the
                    // dirty rect we will need to draw.
                    if !tile.is_valid {
                        dirty_rect = dirty_rect.union(&tile_rect);
                        tile_offset.x = tile_offset.x.min(x);
                        tile_offset.y = tile_offset.y.min(y);
                    }
                }
            }
        }

        self.dirty_region = if dirty_rect.is_empty() {
            None
        } else {
            let dirty_world_rect = world_mapper.map(&dirty_rect).expect("todo");
            Some(DirtyRegion {
                dirty_rect,
                tile_offset,
                dirty_world_rect,
            })
        };
    }
}

/// Maintains a stack of picture and surface information, that
/// is used during the initial picture traversal.
pub struct PictureUpdateState<'a> {
    pub surfaces: &'a mut Vec<SurfaceInfo>,
    surface_stack: Vec<SurfaceIndex>,
    picture_stack: Vec<PictureInfo>,
    /// A stack of currently active tile caches during traversal.
    tile_cache_stack: Vec<TileCache>,
    /// A ref count of how many tile caches on the stack actually
    /// need to have their primitive dependencies updated.
    tile_cache_update_count: usize,
}

impl<'a> PictureUpdateState<'a> {
    pub fn new(surfaces: &'a mut Vec<SurfaceInfo>) -> Self {
        PictureUpdateState {
            surfaces,
            surface_stack: vec![SurfaceIndex(0)],
            picture_stack: Vec::new(),
            tile_cache_stack: Vec::new(),
            tile_cache_update_count: 0,
        }
    }

    /// Return the current surface
    fn current_surface(&self) -> &SurfaceInfo {
        &self.surfaces[self.surface_stack.last().unwrap().0]
    }

    /// Return the current surface (mutable)
    fn current_surface_mut(&mut self) -> &mut SurfaceInfo {
        &mut self.surfaces[self.surface_stack.last().unwrap().0]
    }

    /// Push a new surface onto the update stack.
    fn push_surface(
        &mut self,
        surface: SurfaceInfo,
    ) -> SurfaceIndex {
        let surface_index = SurfaceIndex(self.surfaces.len());
        self.surfaces.push(surface);
        self.surface_stack.push(surface_index);
        surface_index
    }

    /// Pop a surface on the way up the picture traversal
    fn pop_surface(
        &mut self,
    ) {
        self.surface_stack.pop().unwrap();
    }

    /// Return the current picture, or None if stack is empty.
    fn current_picture(&self) -> Option<&PictureInfo> {
        self.picture_stack.last()
    }

    /// Push information about a picture on the update stack
    fn push_picture(
        &mut self,
        info: PictureInfo,
    ) {
        self.picture_stack.push(info);
    }

    /// Pop the picture info off, on the way up the picture traversal
    fn pop_picture(
        &mut self,
    ) -> PictureInfo {
        self.picture_stack.pop().unwrap()
    }

    /// Push a tile cache onto the traversal state.
    pub fn push_tile_cache(
        &mut self,
        tile_cache: TileCache,
    ) {
        if tile_cache.needs_update {
            self.tile_cache_update_count += 1;
        }
        self.tile_cache_stack.push(tile_cache);
    }

    /// Pop a tile cache from the traversal state.
    pub fn pop_tile_cache(
        &mut self,
    ) -> TileCache {
        let tile_cache = self.tile_cache_stack.pop().unwrap();

        if tile_cache.needs_update {
            self.tile_cache_update_count -= 1;
        }

        tile_cache
    }
}

#[derive(Debug, Copy, Clone)]
pub struct SurfaceIndex(pub usize);

pub const ROOT_SURFACE_INDEX: SurfaceIndex = SurfaceIndex(0);

/// Information about an offscreen surface. For now,
/// it contains information about the size and coordinate
/// system of the surface. In the future, it will contain
/// information about the contents of the surface, which
/// will allow surfaces to be cached / retained between
/// frames and display lists.
#[derive(Debug)]
pub struct SurfaceInfo {
    /// A local rect defining the size of this surface, in the
    /// coordinate system of the surface itself.
    pub rect: PictureRect,
    /// Helper structs for mapping local rects in different
    /// coordinate systems into the surface coordinates.
    pub map_local_to_surface: SpaceMapper<LayoutPixel, PicturePixel>,
    /// Defines the positioning node for the surface itself,
    /// and the rasterization root for this surface.
    pub raster_spatial_node_index: SpatialNodeIndex,
    pub surface_spatial_node_index: SpatialNodeIndex,
    /// This is set when the render task is created.
    pub surface: Option<PictureSurface>,
    /// A list of render tasks that are dependencies of this surface.
    pub tasks: Vec<RenderTaskId>,
    /// How much the local surface rect should be inflated (for blur radii).
    pub inflation_factor: f32,
}

impl SurfaceInfo {
    pub fn new(
        surface_spatial_node_index: SpatialNodeIndex,
        raster_spatial_node_index: SpatialNodeIndex,
        inflation_factor: f32,
        world_rect: WorldRect,
        clip_scroll_tree: &ClipScrollTree,
    ) -> Self {
        let map_surface_to_world = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            world_rect,
            clip_scroll_tree,
        );

        let pic_bounds = map_surface_to_world
            .unmap(&map_surface_to_world.bounds)
            .unwrap_or(PictureRect::max_rect());

        let map_local_to_surface = SpaceMapper::new(
            surface_spatial_node_index,
            pic_bounds,
        );

        SurfaceInfo {
            rect: PictureRect::zero(),
            map_local_to_surface,
            surface: None,
            raster_spatial_node_index,
            surface_spatial_node_index,
            tasks: Vec::new(),
            inflation_factor,
        }
    }

    /// Take the set of child render tasks for this surface. This is
    /// used when constructing the render task tree.
    pub fn take_render_tasks(&mut self) -> Vec<RenderTaskId> {
        mem::replace(&mut self.tasks, Vec::new())
    }
}

#[derive(Debug)]
pub struct RasterConfig {
    /// How this picture should be composited into
    /// the parent surface.
    pub composite_mode: PictureCompositeMode,
    /// Index to the surface descriptor for this
    /// picture.
    pub surface_index: SurfaceIndex,
}

/// Specifies how this Picture should be composited
/// onto the target it belongs to.
#[allow(dead_code)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum PictureCompositeMode {
    /// Apply CSS mix-blend-mode effect.
    MixBlend(MixBlendMode),
    /// Apply a CSS filter.
    Filter(FilterOp),
    /// Draw to intermediate surface, copy straight across. This
    /// is used for CSS isolation, and plane splitting.
    Blit,
    /// Used to cache a picture as a series of tiles.
    TileCache {
        clear_color: ColorF,
    },
}

// Stores the location of the picture if it is drawn to
// an intermediate surface. This can be a render task if
// it is not persisted, or a texture cache item if the
// picture is cached in the texture cache.
#[derive(Debug)]
pub enum PictureSurface {
    RenderTask(RenderTaskId),
    TextureCache(RenderTaskCacheEntryHandle),
}

/// Enum value describing the place of a picture in a 3D context.
#[derive(Clone, Debug)]
pub enum Picture3DContext<C> {
    /// The picture is not a part of 3D context sub-hierarchy.
    Out,
    /// The picture is a part of 3D context.
    In {
        /// Additional data per child for the case of this a root of 3D hierarchy.
        root_data: Option<Vec<C>>,
        /// The spatial node index of an "ancestor" element, i.e. one
        /// that establishes the transformed element’s containing block.
        ///
        /// See CSS spec draft for more details:
        /// https://drafts.csswg.org/css-transforms-2/#accumulated-3d-transformation-matrix-computation
        ancestor_index: SpatialNodeIndex,
    },
}

/// Information about a preserve-3D hierarchy child that has been plane-split
/// and ordered according to the view direction.
#[derive(Clone, Debug)]
pub struct OrderedPictureChild {
    pub anchor: usize,
    pub transform_id: TransformPaletteId,
    pub gpu_address: GpuCacheAddress,
}

/// Defines the grouping key for a cluster of primitives in a picture.
/// In future this will also contain spatial grouping details.
#[derive(Hash, Eq, PartialEq, Copy, Clone)]
struct PrimitiveClusterKey {
    /// Grouping primitives by spatial node ensures that we can calculate a local
    /// bounding volume for the cluster, and then transform that by the spatial
    /// node transform once to get an updated bounding volume for the entire cluster.
    spatial_node_index: SpatialNodeIndex,
    /// We want to separate clusters that have different backface visibility properties
    /// so that we can accept / reject an entire cluster at once if the backface is not
    /// visible.
    is_backface_visible: bool,
}

/// Descriptor for a cluster of primitives. For now, this is quite basic but will be
/// extended to handle more spatial clustering of primitives.
pub struct PrimitiveCluster {
    /// The positioning node for this cluster.
    spatial_node_index: SpatialNodeIndex,
    /// Whether this cluster is visible when the position node is a backface.
    is_backface_visible: bool,
    /// The bounding rect of the cluster, in the local space of the spatial node.
    /// This is used to quickly determine the overall bounding rect for a picture
    /// during the first picture traversal, which is needed for local scale
    /// determination, and render task size calculations.
    bounding_rect: LayoutRect,
    /// This flag is set during the first pass picture traversal, depending on whether
    /// the cluster is visible or not. It's read during the second pass when primitives
    /// consult their owning clusters to see if the primitive itself is visible.
    pub is_visible: bool,
}

impl PrimitiveCluster {
    fn new(
        spatial_node_index: SpatialNodeIndex,
        is_backface_visible: bool,
    ) -> Self {
        PrimitiveCluster {
            bounding_rect: LayoutRect::zero(),
            spatial_node_index,
            is_backface_visible,
            is_visible: false,
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct PrimitiveClusterIndex(pub u32);

pub type ClusterRange = ops::Range<u32>;

/// A list of pictures, stored by the PrimitiveList to enable a
/// fast traversal of just the pictures.
pub type PictureList = SmallVec<[PictureIndex; 4]>;

/// A list of primitive instances that are added to a picture
/// This ensures we can keep a list of primitives that
/// are pictures, for a fast initial traversal of the picture
/// tree without walking the instance list.
pub struct PrimitiveList {
    /// The primitive instances, in render order.
    pub prim_instances: Vec<PrimitiveInstance>,
    /// List of pictures that are part of this list.
    /// Used to implement the picture traversal pass.
    pub pictures: PictureList,
    /// List of primitives grouped into clusters.
    pub clusters: SmallVec<[PrimitiveCluster; 4]>,
    /// This maps from the cluster_range in a primitive
    /// instance to a set of cluster(s) that the
    /// primitive instance belongs to.
    pub prim_cluster_map: Vec<PrimitiveClusterIndex>,
}

impl PrimitiveList {
    /// Construct an empty primitive list. This is
    /// just used during the take_context / restore_context
    /// borrow check dance, which will be removed as the
    /// picture traversal pass is completed.
    pub fn empty() -> Self {
        PrimitiveList {
            prim_instances: Vec::new(),
            pictures: SmallVec::new(),
            clusters: SmallVec::new(),
            prim_cluster_map: Vec::new(),
        }
    }

    /// Construct a new prim list from a list of instances
    /// in render order. This does some work during scene
    /// building which makes the frame building traversals
    /// significantly faster.
    pub fn new(
        mut prim_instances: Vec<PrimitiveInstance>,
        resources: &DocumentResources
    ) -> Self {
        let mut pictures = SmallVec::new();
        let mut clusters_map = FastHashMap::default();
        let mut clusters: SmallVec<[PrimitiveCluster; 4]> = SmallVec::new();
        let mut prim_cluster_map = Vec::new();

        // Walk the list of primitive instances and extract any that
        // are pictures.
        for prim_instance in &mut prim_instances {
            // Check if this primitive is a picture. In future we should
            // remove this match and embed this info directly in the primitive instance.
            let is_pic = match prim_instance.kind {
                PrimitiveInstanceKind::Picture { pic_index, .. } => {
                    pictures.push(pic_index);
                    true
                }
                _ => {
                    false
                }
            };

            let prim_data = match prim_instance.kind {
                PrimitiveInstanceKind::Picture { data_handle, .. } |
                PrimitiveInstanceKind::LineDecoration { data_handle, .. } |
                PrimitiveInstanceKind::NormalBorder { data_handle, .. } |
                PrimitiveInstanceKind::ImageBorder { data_handle, .. } |
                PrimitiveInstanceKind::Rectangle { data_handle, .. } |
                PrimitiveInstanceKind::YuvImage { data_handle, .. } |
                PrimitiveInstanceKind::Image { data_handle, .. } |
                PrimitiveInstanceKind::Clear { data_handle, .. } => {
                    &resources.prim_interner[data_handle]
                }
                PrimitiveInstanceKind::LinearGradient { data_handle, .. } => {
                    &resources.linear_grad_interner[data_handle]
                }
                PrimitiveInstanceKind::RadialGradient { data_handle, ..} => {
                    &resources.radial_grad_interner[data_handle]
                }
                PrimitiveInstanceKind::TextRun { data_handle, .. } => {
                    &resources.text_run_interner[data_handle]
                }
            };

            // Get the key for the cluster that this primitive should
            // belong to.
            let key = PrimitiveClusterKey {
                spatial_node_index: prim_instance.spatial_node_index,
                is_backface_visible: prim_data.is_backface_visible,
            };

            // Find the cluster, or create a new one.
            let cluster_index = *clusters_map
                .entry(key)
                .or_insert_with(|| {
                    let index = clusters.len();
                    clusters.push(PrimitiveCluster::new(
                        prim_instance.spatial_node_index,
                        prim_data.is_backface_visible,
                    ));
                    index
                }
            );

            // Pictures don't have a known static local bounding rect (they are
            // calculated during the picture traversal dynamically). If not
            // a picture, include a minimal bounding rect in the cluster bounds.
            let cluster = &mut clusters[cluster_index];
            if !is_pic {
                let prim_rect = LayoutRect::new(
                    prim_instance.prim_origin,
                    prim_data.prim_size,
                );
                let clip_rect = prim_data
                    .prim_relative_clip_rect
                    .translate(&prim_instance.prim_origin.to_vector());
                let culling_rect = clip_rect
                    .intersection(&prim_rect)
                    .unwrap_or(LayoutRect::zero());

                cluster.bounding_rect = cluster.bounding_rect.union(&culling_rect);
            }

            // Define a range of clusters that this primitive belongs to. For now, this
            // seems like overkill, since a primitive only ever belongs to one cluster.
            // However, in the future the clusters will include spatial information. It
            // will often be the case that a primitive may overlap more than one cluster,
            // and belong to several.
            let start = prim_cluster_map.len() as u32;
            let cluster_range = ClusterRange {
                start,
                end: start + 1,
            };

            // Store the cluster index in the map, and the range in the instance.
            prim_cluster_map.push(PrimitiveClusterIndex(cluster_index as u32));
            prim_instance.cluster_range = cluster_range;
        }

        PrimitiveList {
            prim_instances,
            pictures,
            clusters,
            prim_cluster_map,
        }
    }
}

pub struct PicturePrimitive {
    /// List of primitives, and associated info for this picture.
    pub prim_list: PrimitiveList,

    pub state: Option<(PictureState, PictureContext)>,

    // The pipeline that the primitives on this picture belong to.
    pub pipeline_id: PipelineId,

    // If true, apply the local clip rect to primitive drawn
    // in this picture.
    pub apply_local_clip_rect: bool,

    // If a mix-blend-mode, contains the render task for
    // the readback of the framebuffer that we use to sample
    // from in the mix-blend-mode shader.
    // For drop-shadow filter, this will store the original
    // picture task which would be rendered on screen after
    // blur pass.
    pub secondary_render_task_id: Option<RenderTaskId>,
    /// How this picture should be composited.
    /// If None, don't composite - just draw directly on parent surface.
    pub requested_composite_mode: Option<PictureCompositeMode>,
    /// Requested rasterization space for this picture. It is
    /// a performance hint only.
    pub requested_raster_space: RasterSpace,

    pub raster_config: Option<RasterConfig>,
    pub context_3d: Picture3DContext<OrderedPictureChild>,

    // If requested as a frame output (for rendering
    // pages to a texture), this is the pipeline this
    // picture is the root of.
    pub frame_output_pipeline_id: Option<PipelineId>,
    // An optional cache handle for storing extra data
    // in the GPU cache, depending on the type of
    // picture.
    pub extra_gpu_data_handle: GpuCacheHandle,

    /// The spatial node index of this picture when it is
    /// composited into the parent picture.
    pub spatial_node_index: SpatialNodeIndex,

    /// The local rect of this picture. It is built
    /// dynamically during the first picture traversal.
    pub local_rect: LayoutRect,

    /// Local clip rect for this picture.
    pub local_clip_rect: LayoutRect,

    /// A descriptor for this surface that can be used as a cache key.
    surface_desc: Option<SurfaceDescriptor>,

    pub gpu_location: GpuCacheHandle,

    /// If Some(..) the tile cache that is associated with this picture.
    pub tile_cache: Option<TileCache>,
}

impl PicturePrimitive {
    fn resolve_scene_properties(&mut self, properties: &SceneProperties) -> bool {
        match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(ref mut filter)) => {
                match *filter {
                    FilterOp::Opacity(ref binding, ref mut value) => {
                        *value = properties.resolve_float(binding);
                    }
                    _ => {}
                }

                filter.is_visible()
            }
            _ => true,
        }
    }

    fn is_visible(&self) -> bool {
        match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(ref filter)) => {
                filter.is_visible()
            }
            _ => true,
        }
    }

    /// Destroy an existing picture. This is called just before
    /// a frame builder is replaced with a newly built scene. It
    /// gives a picture a chance to retain any cached tiles that
    /// may be useful during the next scene build.
    pub fn destroy(
        mut self,
        retained_tiles: &mut FastHashMap<TileDescriptor, TextureCacheHandle>,
    ) {
        if let Some(tile_cache) = self.tile_cache.take() {
            debug_assert!(tile_cache.old_tiles.is_empty());
            for tile in tile_cache.tiles {
                retained_tiles.extend(tile.destroy());
            }
        }
    }

    pub fn new_image(
        requested_composite_mode: Option<PictureCompositeMode>,
        context_3d: Picture3DContext<OrderedPictureChild>,
        pipeline_id: PipelineId,
        frame_output_pipeline_id: Option<PipelineId>,
        apply_local_clip_rect: bool,
        requested_raster_space: RasterSpace,
        prim_list: PrimitiveList,
        spatial_node_index: SpatialNodeIndex,
        local_clip_rect: LayoutRect,
        clip_store: &ClipStore,
    ) -> Self {
        // For now, only create a cache descriptor for blur filters (which
        // includes text shadows). We can incrementally expand this to
        // handle more composite modes.
        let create_cache_descriptor = match requested_composite_mode {
            Some(PictureCompositeMode::Filter(FilterOp::Blur(blur_radius))) => {
                blur_radius > 0.0
            }
            Some(_) | None => {
                false
            }
        };

        let surface_desc = if create_cache_descriptor {
            SurfaceDescriptor::new(
                &prim_list.prim_instances,
                spatial_node_index,
                clip_store,
            )
        } else {
            None
        };

        let tile_cache = match requested_composite_mode {
            Some(PictureCompositeMode::TileCache { .. }) => {
                Some(TileCache::new())
            }
            Some(_) | None => {
                None
            }
        };

        PicturePrimitive {
            surface_desc,
            prim_list,
            state: None,
            secondary_render_task_id: None,
            requested_composite_mode,
            raster_config: None,
            context_3d,
            frame_output_pipeline_id,
            extra_gpu_data_handle: GpuCacheHandle::new(),
            apply_local_clip_rect,
            pipeline_id,
            requested_raster_space,
            spatial_node_index,
            local_rect: LayoutRect::zero(),
            local_clip_rect,
            gpu_location: GpuCacheHandle::new(),
            tile_cache,
        }
    }

    pub fn take_context(
        &mut self,
        pic_index: PictureIndex,
        surface_spatial_node_index: SpatialNodeIndex,
        raster_spatial_node_index: SpatialNodeIndex,
        surface_index: SurfaceIndex,
        parent_allows_subpixel_aa: bool,
        frame_state: &mut FrameBuildingState,
        frame_context: &FrameBuildingContext,
    ) -> Option<(PictureContext, PictureState, PrimitiveList)> {
        if !self.is_visible() {
            return None;
        }

        // Work out the dirty world rect for this picture.
        let dirty_world_rect = match self.tile_cache {
            Some(ref tile_cache) => {
                // If a tile cache is present, extract the dirty
                // world rect from the dirty region. If there is
                // no dirty region there is nothing to render.
                // TODO(gw): We could early out here in that case?
                tile_cache
                    .dirty_region
                    .as_ref()
                    .map_or(WorldRect::zero(), |region| {
                        region.dirty_world_rect
                    })
            }
            None => {
                // No tile cache - just assume the world rect of the screen.
                frame_context.screen_world_rect
            }
        };

        // Extract the raster and surface spatial nodes from the raster
        // config, if this picture establishes a surface. Otherwise just
        // pass in the spatial node indices from the parent context.
        let (raster_spatial_node_index, surface_spatial_node_index, surface_index) = match self.raster_config {
            Some(ref raster_config) => {
                let surface = &frame_state.surfaces[raster_config.surface_index.0];

                (surface.raster_spatial_node_index, self.spatial_node_index, raster_config.surface_index)
            }
            None => {
                (raster_spatial_node_index, surface_spatial_node_index, surface_index)
            }
        };

        if self.raster_config.is_some() {
            frame_state.clip_store
                .push_surface(surface_spatial_node_index);
        }

        let map_pic_to_world = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            dirty_world_rect,
            frame_context.clip_scroll_tree,
        );

        let pic_bounds = map_pic_to_world.unmap(&map_pic_to_world.bounds)
                                         .unwrap_or(PictureRect::max_rect());

        let map_local_to_pic = SpaceMapper::new(
            surface_spatial_node_index,
            pic_bounds,
        );

        let (map_raster_to_world, map_pic_to_raster) = create_raster_mappers(
            surface_spatial_node_index,
            raster_spatial_node_index,
            dirty_world_rect,
            frame_context.clip_scroll_tree,
        );

        let plane_splitter = match self.context_3d {
            Picture3DContext::Out => {
                None
            }
            Picture3DContext::In { root_data: Some(_), .. } => {
                Some(PlaneSplitter::new())
            }
            Picture3DContext::In { root_data: None, .. } => {
                None
            }
        };

        let state = PictureState {
            is_cacheable: true,
            map_local_to_pic,
            map_pic_to_world,
            map_pic_to_raster,
            map_raster_to_world,
            plane_splitter,
        };

        // Disallow subpixel AA if an intermediate surface is needed.
        // TODO(lsalzman): allow overriding parent if intermediate surface is opaque
        let allow_subpixel_aa = match self.raster_config {
            Some(RasterConfig { composite_mode: PictureCompositeMode::TileCache { clear_color, .. }, .. }) => {
                // If the tile cache has an opaque background, then it's fine to use
                // subpixel rendering (this is the common case).
                clear_color.a >= 1.0
            },
            Some(_) => {
                false
            }
            None => {
                true
            }
        };
        // Still disable subpixel AA if parent forbids it
        let allow_subpixel_aa = parent_allows_subpixel_aa && allow_subpixel_aa;

        let context = PictureContext {
            pic_index,
            pipeline_id: self.pipeline_id,
            apply_local_clip_rect: self.apply_local_clip_rect,
            allow_subpixel_aa,
            is_passthrough: self.raster_config.is_none(),
            raster_space: self.requested_raster_space,
            raster_spatial_node_index,
            surface_spatial_node_index,
            surface_index,
            dirty_world_rect,
        };

        let prim_list = mem::replace(&mut self.prim_list, PrimitiveList::empty());

        Some((context, state, prim_list))
    }

    pub fn restore_context(
        &mut self,
        prim_list: PrimitiveList,
        context: PictureContext,
        state: PictureState,
        frame_state: &mut FrameBuildingState,
    ) -> Option<ClipNodeCollector> {
        self.prim_list = prim_list;
        self.state = Some((state, context));

        match self.raster_config {
            Some(ref raster_config) => {
                let local_rect = frame_state.surfaces[raster_config.surface_index.0].rect;
                let local_rect = LayoutRect::from_untyped(&local_rect.to_untyped());

                // If the local rect changed (due to transforms in child primitives) then
                // invalidate the GPU cache location to re-upload the new local rect
                // and stretch size. Drop shadow filters also depend on the local rect
                // size for the extra GPU cache data handle.
                // TODO(gw): In future, if we support specifying a flag which gets the
                //           stretch size from the segment rect in the shaders, we can
                //           remove this invalidation here completely.
                if self.local_rect != local_rect {
                    frame_state.gpu_cache.invalidate(&self.gpu_location);
                    if let PictureCompositeMode::Filter(FilterOp::DropShadow(..)) = raster_config.composite_mode {
                        frame_state.gpu_cache.invalidate(&self.extra_gpu_data_handle);
                    }
                }

                self.local_rect = local_rect;

                Some(frame_state.clip_store.pop_surface())
            }
            None => {
                None
            }
        }
    }

    pub fn take_state_and_context(&mut self) -> (PictureState, PictureContext) {
        self.state.take().expect("bug: no state present!")
    }

    /// Add a primitive instance to the plane splitter. The function would generate
    /// an appropriate polygon, clip it against the frustum, and register with the
    /// given plane splitter.
    pub fn add_split_plane(
        splitter: &mut PlaneSplitter,
        transforms: &TransformPalette,
        prim_instance: &PrimitiveInstance,
        original_local_rect: LayoutRect,
        world_rect: WorldRect,
        plane_split_anchor: usize,
    ) -> bool {
        let transform = transforms
            .get_world_transform(prim_instance.spatial_node_index);
        let matrix = transform.cast();

        // Apply the local clip rect here, before splitting. This is
        // because the local clip rect can't be applied in the vertex
        // shader for split composites, since we are drawing polygons
        // rather that rectangles. The interpolation still works correctly
        // since we determine the UVs by doing a bilerp with a factor
        // from the original local rect.
        let local_rect = match original_local_rect
            .intersection(&prim_instance.combined_local_clip_rect)
        {
            Some(rect) => rect.cast(),
            None => return false,
        };
        let world_rect = world_rect.cast();

        match transform.transform_kind() {
            TransformedRectKind::AxisAligned => {
                let inv_transform = transforms
                    .get_world_inv_transform(prim_instance.spatial_node_index);
                let polygon = Polygon::from_transformed_rect_with_inverse(
                    local_rect,
                    &matrix,
                    &inv_transform.cast(),
                    plane_split_anchor,
                ).unwrap();
                splitter.add(polygon);
            }
            TransformedRectKind::Complex => {
                let mut clipper = Clipper::new();
                let results = clipper.clip_transformed(
                    Polygon::from_rect(
                        local_rect,
                        plane_split_anchor,
                    ),
                    &matrix,
                    Some(world_rect),
                );
                if let Ok(results) = results {
                    for poly in results {
                        splitter.add(poly);
                    }
                }
            }
        }

        true
    }

    pub fn resolve_split_planes(
        &mut self,
        splitter: &mut PlaneSplitter,
        frame_state: &mut FrameBuildingState,
        clip_scroll_tree: &ClipScrollTree,
    ) {
        let ordered = match self.context_3d {
            Picture3DContext::In { root_data: Some(ref mut list), .. } => list,
            _ => panic!("Expected to find 3D context root"),
        };
        ordered.clear();

        // Process the accumulated split planes and order them for rendering.
        // Z axis is directed at the screen, `sort` is ascending, and we need back-to-front order.
        for poly in splitter.sort(vec3(0.0, 0.0, 1.0)) {
            let spatial_node_index = self.prim_list.prim_instances[poly.anchor].spatial_node_index;

            let transform = frame_state.transforms.get_world_inv_transform(spatial_node_index);
            let transform_id = frame_state.transforms.get_id(
                spatial_node_index,
                ROOT_SPATIAL_NODE_INDEX,
                clip_scroll_tree,
            );

            let local_points = [
                transform.transform_point3d(&poly.points[0].cast()).unwrap(),
                transform.transform_point3d(&poly.points[1].cast()).unwrap(),
                transform.transform_point3d(&poly.points[2].cast()).unwrap(),
                transform.transform_point3d(&poly.points[3].cast()).unwrap(),
            ];
            let gpu_blocks = [
                [local_points[0].x, local_points[0].y, local_points[1].x, local_points[1].y].into(),
                [local_points[2].x, local_points[2].y, local_points[3].x, local_points[3].y].into(),
            ];
            let gpu_handle = frame_state.gpu_cache.push_per_frame_blocks(&gpu_blocks);
            let gpu_address = frame_state.gpu_cache.get_address(&gpu_handle);

            ordered.push(OrderedPictureChild {
                anchor: poly.anchor,
                transform_id,
                gpu_address,
            });
        }
    }

    /// Called during initial picture traversal, before we know the
    /// bounding rect of children. It is possible to determine the
    /// surface / raster config now though.
    pub fn pre_update(
        &mut self,
        state: &mut PictureUpdateState,
        frame_context: &FrameBuildingContext,
    ) -> Option<PictureList> {
        // Reset raster config in case we early out below.
        self.raster_config = None;

        // Resolve animation properties, and early out if the filter
        // properties make this picture invisible.
        if !self.resolve_scene_properties(frame_context.scene_properties) {
            return None;
        }

        // Push information about this pic on stack for children to read.
        state.push_picture(PictureInfo {
            spatial_node_index: self.spatial_node_index,
        });

        // See if this picture actually needs a surface for compositing.
        let actual_composite_mode = match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(filter)) if filter.is_noop() => None,
            mode => mode,
        };

        if let Some(composite_mode) = actual_composite_mode {
            // Retrieve the positioning node information for the parent surface.
            let parent_raster_spatial_node_index = state.current_surface().raster_spatial_node_index;
            let surface_spatial_node_index = self.spatial_node_index;

            // Check if there is perspective, and thus whether a new
            // rasterization root should be established.
            let xf = frame_context.clip_scroll_tree.get_relative_transform(
                parent_raster_spatial_node_index,
                surface_spatial_node_index,
            ).expect("BUG: unable to get relative transform");

            // TODO(gw): A temporary hack here to revert behavior to
            //           always raster in screen-space. This is not
            //           a problem yet, since we're not taking advantage
            //           of this for caching yet. This is a workaround
            //           for some existing issues with handling scale
            //           when rasterizing in local space mode. Once
            //           the fixes for those are in-place, we can
            //           remove this hack!
            //let local_scale = raster_space.local_scale();
            // let wants_raster_root = xf.has_perspective_component() ||
            //                         local_scale.is_some();
            let establishes_raster_root = xf.has_perspective_component();

            // TODO(gw): For now, we always raster in screen space. Soon,
            //           we will be able to respect the requested raster
            //           space, and/or override the requested raster root
            //           if it makes sense to.
            let raster_space = RasterSpace::Screen;

            let raster_spatial_node_index = if establishes_raster_root {
                surface_spatial_node_index
            } else {
                parent_raster_spatial_node_index
            };

            let inflation_factor = match composite_mode {
                PictureCompositeMode::Filter(FilterOp::Blur(blur_radius)) => {
                    // The amount of extra space needed for primitives inside
                    // this picture to ensure the visibility check is correct.
                    BLUR_SAMPLE_SCALE * blur_radius
                }
                _ => {
                    0.0
                }
            };

            let surface_index = state.push_surface(
                SurfaceInfo::new(
                    surface_spatial_node_index,
                    raster_spatial_node_index,
                    inflation_factor,
                    frame_context.screen_world_rect,
                    &frame_context.clip_scroll_tree,
                )
            );

            self.raster_config = Some(RasterConfig {
                composite_mode,
                surface_index,
            });

            // If we have a tile cache for this picture, see if any of the
            // relative transforms have changed, which means we need to
            // re-map the dependencies of any child primitives.
            if let Some(mut tile_cache) = self.tile_cache.take() {
                tile_cache.update_transforms(
                    surface_spatial_node_index,
                    raster_spatial_node_index,
                    raster_space,
                    frame_context,
                );

                state.push_tile_cache(tile_cache);
            }

            // If we have a cache key / descriptor for this surface,
            // update any transforms it cares about.
            if let Some(ref mut surface_desc) = self.surface_desc {
                surface_desc.update(
                    surface_spatial_node_index,
                    raster_spatial_node_index,
                    frame_context.clip_scroll_tree,
                    raster_space,
                );
            }
        }

        Some(mem::replace(&mut self.prim_list.pictures, SmallVec::new()))
    }

    /// Update the primitive dependencies for any active tile caches,
    /// but only *if* the transforms have made the mappings out of date.
    pub fn update_prim_dependencies(
        &self,
        state: &mut PictureUpdateState,
        frame_context: &FrameBuildingContext,
        resource_cache: &mut ResourceCache,
        resources: &FrameResources,
        pictures: &[PicturePrimitive],
        clip_store: &ClipStore,
        opacity_binding_store: &OpacityBindingStorage,
        image_instances: &ImageInstanceStorage,
    ) {
        if state.tile_cache_update_count == 0 {
            return;
        }

        let surface_spatial_node_index = state.current_surface().surface_spatial_node_index;

        for prim_instance in &self.prim_list.prim_instances {
            for tile_cache in &mut state.tile_cache_stack {
                tile_cache.update_prim_dependencies(
                    prim_instance,
                    surface_spatial_node_index,
                    &frame_context.clip_scroll_tree,
                    resources,
                    &clip_store.clip_chain_nodes,
                    pictures,
                    resource_cache,
                    opacity_binding_store,
                    image_instances,
                );
            }
        }
    }

    /// Called after updating child pictures during the initial
    /// picture traversal.
    pub fn post_update(
        &mut self,
        child_pictures: PictureList,
        state: &mut PictureUpdateState,
        frame_context: &FrameBuildingContext,
        resource_cache: &mut ResourceCache,
        gpu_cache: &mut GpuCache,
        retained_tiles: &mut FastHashMap<TileDescriptor, TextureCacheHandle>,
    ) {
        // Pop the state information about this picture.
        state.pop_picture();

        for cluster in &mut self.prim_list.clusters {
            // Skip the cluster if backface culled.
            if !cluster.is_backface_visible {
                let containing_block_index = match self.context_3d {
                    Picture3DContext::Out => {
                        state.current_picture().map_or(ROOT_SPATIAL_NODE_INDEX, |info| {
                            info.spatial_node_index
                        })
                    }
                    Picture3DContext::In { root_data: Some(_), ancestor_index } => {
                        ancestor_index
                    }
                    Picture3DContext::In { root_data: None, ancestor_index } => {
                        ancestor_index
                    }
                };

                let map_local_to_containing_block: SpaceMapper<LayoutPixel, LayoutPixel> = SpaceMapper::new_with_target(
                    containing_block_index,
                    cluster.spatial_node_index,
                    LayoutRect::zero(),     // bounds aren't going to be used for this mapping
                    &frame_context.clip_scroll_tree,
                );

                match map_local_to_containing_block.visible_face() {
                    VisibleFace::Back => continue,
                    VisibleFace::Front => {}
                }
            }

            // No point including this cluster if it can't be transformed
            let spatial_node = &frame_context
                .clip_scroll_tree
                .spatial_nodes[cluster.spatial_node_index.0];
            if !spatial_node.invertible {
                continue;
            }

            // Map the cluster bounding rect into the space of the surface, and
            // include it in the surface bounding rect.
            let surface = state.current_surface_mut();
            surface.map_local_to_surface.set_target_spatial_node(
                cluster.spatial_node_index,
                frame_context.clip_scroll_tree,
            );

            // Mark the cluster visible, since it passed the invertible and
            // backface checks. In future, this will include spatial clustering
            // which will allow the frame building code to skip most of the
            // current per-primitive culling code.
            cluster.is_visible = true;
            if let Some(cluster_rect) = surface.map_local_to_surface.map(&cluster.bounding_rect) {
                surface.rect = surface.rect.union(&cluster_rect);
            }
        }

        // Inflate the local bounding rect if required by the filter effect.
        let inflation_size = match self.raster_config {
            Some(RasterConfig { composite_mode: PictureCompositeMode::Filter(FilterOp::Blur(blur_radius)), .. }) |
            Some(RasterConfig { composite_mode: PictureCompositeMode::Filter(FilterOp::DropShadow(_, blur_radius, _)), .. }) => {
                Some((blur_radius * BLUR_SAMPLE_SCALE).ceil())
            }
            _ => {
                None
            }
        };
        if let Some(inflation_size) = inflation_size {
            let surface = state.current_surface_mut();
            surface.rect = surface.rect.inflate(inflation_size, inflation_size);
        }

        // Restore the pictures list used during recursion.
        self.prim_list.pictures = child_pictures;

        // If this picture establishes a surface, then map the surface bounding
        // rect into the parent surface coordinate space, and propagate that up
        // to the parent.
        if let Some(ref raster_config) = self.raster_config {
            let surface_rect = state.current_surface().rect;

            if let PictureCompositeMode::TileCache { .. } = raster_config.composite_mode {
                let mut tile_cache = state.pop_tile_cache();

                // Build the dirty region(s) for this tile cache.
                tile_cache.build_dirty_regions(
                    self.spatial_node_index,
                    frame_context,
                    resource_cache,
                    gpu_cache,
                    retained_tiles,
                );

                self.tile_cache = Some(tile_cache);
            }

            let mut surface_rect = TypedRect::from_untyped(&surface_rect.to_untyped());

            // Pop this surface from the stack
            state.pop_surface();

            // Drop shadows draw both a content and shadow rect, so need to expand the local
            // rect of any surfaces to be composited in parent surfaces correctly.
            if let PictureCompositeMode::Filter(FilterOp::DropShadow(offset, ..)) = raster_config.composite_mode {
                let content_rect = surface_rect;
                let shadow_rect = surface_rect.translate(&offset);
                surface_rect = content_rect.union(&shadow_rect);
            }

            // Propagate up to parent surface, now that we know this surface's static rect
            let parent_surface = state.current_surface_mut();
            parent_surface.map_local_to_surface.set_target_spatial_node(
                self.spatial_node_index,
                frame_context.clip_scroll_tree,
            );
            if let Some(parent_surface_rect) = parent_surface
                .map_local_to_surface
                .map(&surface_rect) {
                parent_surface.rect = parent_surface.rect.union(&parent_surface_rect);
            }
        }
    }

    pub fn prepare_for_render(
        &mut self,
        pic_index: PictureIndex,
        prim_instance: &PrimitiveInstance,
        prim_local_rect: &LayoutRect,
        clipped_prim_bounding_rect: WorldRect,
        surface_index: SurfaceIndex,
        frame_context: &FrameBuildingContext,
        frame_state: &mut FrameBuildingState,
    ) -> bool {
        let (mut pic_state_for_children, pic_context) = self.take_state_and_context();

        if let Some(ref mut splitter) = pic_state_for_children.plane_splitter {
            self.resolve_split_planes(
                splitter,
                frame_state,
                frame_context.clip_scroll_tree,
            );
        }

        let raster_config = match self.raster_config {
            Some(ref mut raster_config) => raster_config,
            None => {
                return true
            }
        };

        let (raster_spatial_node_index, child_tasks) = {
            let surface_info = &mut frame_state.surfaces[raster_config.surface_index.0];
            (surface_info.raster_spatial_node_index, surface_info.take_render_tasks())
        };
        let surfaces = &mut frame_state.surfaces;

        let (map_raster_to_world, map_pic_to_raster) = create_raster_mappers(
            prim_instance.spatial_node_index,
            raster_spatial_node_index,
            pic_context.dirty_world_rect,
            frame_context.clip_scroll_tree,
        );

        let pic_rect = PictureRect::from_untyped(&prim_local_rect.to_untyped());

        let (clipped, unclipped) = match get_raster_rects(
            pic_rect,
            &map_pic_to_raster,
            &map_raster_to_world,
            clipped_prim_bounding_rect,
            frame_context.device_pixel_scale,
        ) {
            Some(info) => info,
            None => return false,
        };
        let transform = map_pic_to_raster.get_transform();

        // TODO(gw): Almost all of the Picture types below use extra_gpu_cache_data
        //           to store the same type of data. The exception is the filter
        //           with a ColorMatrix, which stores the color matrix here. It's
        //           probably worth tidying this code up to be a bit more consistent.
        //           Perhaps store the color matrix after the common data, even though
        //           it's not used by that shader.

        let surface = match raster_config.composite_mode {
            PictureCompositeMode::TileCache { clear_color, .. } => {
                let tile_cache = self.tile_cache.as_mut().unwrap();

                // Build the render task for a tile cache picture, if there is
                // any dirty rect.

                match tile_cache.dirty_region {
                    Some(ref dirty_region) => {
                        // Texture cache descriptor for each tile.
                        let descriptor = ImageDescriptor::new(
                            TILE_SIZE_DP,
                            TILE_SIZE_DP,
                            ImageFormat::BGRA8,
                            false,          // TODO(gw): Detect when background color is opaque!
                            false,
                        );

                        // Get a picture rect, expanded to tile boundaries.
                        let p0 = pic_rect.origin;
                        let p1 = pic_rect.bottom_right();
                        let local_tile_size = tile_cache.local_tile_size;
                        let aligned_pic_rect = PictureRect::from_floats(
                            (p0.x / local_tile_size.width).floor() * local_tile_size.width,
                            (p0.y / local_tile_size.height).floor() * local_tile_size.height,
                            (p1.x / local_tile_size.width).ceil() * local_tile_size.width,
                            (p1.y / local_tile_size.height).ceil() * local_tile_size.height,
                        );

                        let mut blits = Vec::new();

                        // Step through each tile and build the dirty rect
                        for y in 0 .. tile_cache.tile_rect.size.height {
                            for x in 0 .. tile_cache.tile_rect.size.width {
                                let i = y * tile_cache.tile_rect.size.width + x;
                                let tile = &mut tile_cache.tiles[i as usize];

                                // If tile is invalidated, and on-screen, then we will
                                // need to rasterize it.
                                if !tile.is_valid && tile.is_visible && tile.in_use {
                                    // Notify the texture cache that we want to use this handle
                                    // and make sure it is allocated.
                                    frame_state.resource_cache.texture_cache.update(
                                        &mut tile.handle,
                                        descriptor,
                                        TextureFilter::Linear,
                                        None,
                                        [0.0; 3],
                                        DirtyRect::All,
                                        frame_state.gpu_cache,
                                        None,
                                        UvRectKind::Rect,
                                        Eviction::Eager,
                                    );

                                    let cache_item = frame_state
                                        .resource_cache
                                        .get_texture_cache_item(&tile.handle);

                                    // Set up the blit command now that we know where the dest
                                    // rect is in the texture cache.
                                    let offset = DeviceIntPoint::new(
                                        (x - dirty_region.tile_offset.x) * TILE_SIZE_DP,
                                        (y - dirty_region.tile_offset.y) * TILE_SIZE_DP,
                                    );

                                    blits.push(TileBlit {
                                        target: cache_item,
                                        offset,
                                    });

                                    tile.is_valid = true;
                                }
                            }
                        }

                        // We want to clip the drawing of this and any children to the
                        // dirty rect.
                        let clipped_rect = dirty_region.dirty_world_rect;

                        let (clipped, unclipped) = match get_raster_rects(
                            aligned_pic_rect,
                            &map_pic_to_raster,
                            &map_raster_to_world,
                            clipped_rect,
                            frame_context.device_pixel_scale,
                        ) {
                            Some(info) => info,
                            None => {
                                return false;
                            }
                        };

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, clipped.size),
                            unclipped.size,
                            pic_index,
                            clipped.origin,
                            child_tasks,
                            UvRectKind::Rect,
                            pic_context.raster_spatial_node_index,
                            Some(clear_color),
                            blits,
                        );

                        let render_task_id = frame_state.render_tasks.add(picture_task);
                        surfaces[surface_index.0].tasks.push(render_task_id);

                        PictureSurface::RenderTask(render_task_id)
                    }
                    None => {
                        // None of the tiles have changed, so we can skip any drawing!
                        return true;
                    }
                }
            }
            PictureCompositeMode::Filter(FilterOp::Blur(blur_radius)) => {
                let blur_std_deviation = blur_radius * frame_context.device_pixel_scale.0;
                let blur_range = (blur_std_deviation * BLUR_SAMPLE_SCALE).ceil() as i32;

                // We need to choose whether to cache this picture, or draw
                // it into a temporary render target each frame. If we draw
                // it into a persistently cached texture, then we want to
                // draw the whole picture, without clipping it to the screen
                // dimensions, so that it can be reused as it scrolls into
                // view etc. However, if the unclipped size of the surface is
                // too big, then it will be very expensive to draw, and may
                // even be bigger than the maximum hardware render target
                // size. In these cases, it's probably best to not cache the
                // picture, and just draw a minimal portion of the picture
                // (clipped to screen bounds) to a temporary target each frame.

                // TODO(gw): This size is quite arbitrary - we should do some
                //           profiling / telemetry to see when it makes sense
                //           to cache a picture.
                const MAX_CACHE_SIZE: f32 = 2048.0;
                let too_big_to_cache = unclipped.size.width > MAX_CACHE_SIZE ||
                                       unclipped.size.height > MAX_CACHE_SIZE;

                // If we can't create a valid cache key for this descriptor (e.g.
                // due to it referencing old non-interned style primitives), then
                // don't try to cache it.
                let has_valid_cache_key = self.surface_desc.is_some();

                if !has_valid_cache_key ||
                   too_big_to_cache ||
                   !pic_state_for_children.is_cacheable {
                    // The clipped field is the part of the picture that is visible
                    // on screen. The unclipped field is the screen-space rect of
                    // the complete picture, if no screen / clip-chain was applied
                    // (this includes the extra space for blur region). To ensure
                    // that we draw a large enough part of the picture to get correct
                    // blur results, inflate that clipped area by the blur range, and
                    // then intersect with the total screen rect, to minimize the
                    // allocation size.
                    let device_rect = clipped
                        .inflate(blur_range, blur_range)
                        .intersection(&unclipped.to_i32())
                        .unwrap();

                    let uv_rect_kind = calculate_uv_rect_kind(
                        &pic_rect,
                        &transform,
                        &device_rect,
                        frame_context.device_pixel_scale,
                    );

                    let picture_task = RenderTask::new_picture(
                        RenderTaskLocation::Dynamic(None, device_rect.size),
                        unclipped.size,
                        pic_index,
                        device_rect.origin,
                        child_tasks,
                        uv_rect_kind,
                        pic_context.raster_spatial_node_index,
                        None,
                        Vec::new(),
                    );

                    let picture_task_id = frame_state.render_tasks.add(picture_task);

                    let blur_render_task = RenderTask::new_blur(
                        blur_std_deviation,
                        picture_task_id,
                        frame_state.render_tasks,
                        RenderTargetKind::Color,
                        ClearMode::Transparent,
                    );

                    let render_task_id = frame_state.render_tasks.add(blur_render_task);

                    surfaces[surface_index.0].tasks.push(render_task_id);

                    PictureSurface::RenderTask(render_task_id)
                } else {
                    // Request a render task that will cache the output in the
                    // texture cache.
                    let device_rect = unclipped.to_i32();

                    let uv_rect_kind = calculate_uv_rect_kind(
                        &pic_rect,
                        &transform,
                        &device_rect,
                        frame_context.device_pixel_scale,
                    );

                    // TODO(gw): Probably worth changing the render task caching API
                    //           so that we don't need to always clone the key.
                    let cache_key = self.surface_desc
                        .as_ref()
                        .expect("bug: no cache key for surface")
                        .cache_key
                        .clone();

                    let cache_item = frame_state.resource_cache.request_render_task(
                        RenderTaskCacheKey {
                            size: device_rect.size,
                            kind: RenderTaskCacheKeyKind::Picture(cache_key),
                        },
                        frame_state.gpu_cache,
                        frame_state.render_tasks,
                        None,
                        false,
                        |render_tasks| {
                            let picture_task = RenderTask::new_picture(
                                RenderTaskLocation::Dynamic(None, device_rect.size),
                                unclipped.size,
                                pic_index,
                                device_rect.origin,
                                child_tasks,
                                uv_rect_kind,
                                pic_context.raster_spatial_node_index,
                                None,
                                Vec::new(),
                            );

                            let picture_task_id = render_tasks.add(picture_task);

                            let blur_render_task = RenderTask::new_blur(
                                blur_std_deviation,
                                picture_task_id,
                                render_tasks,
                                RenderTargetKind::Color,
                                ClearMode::Transparent,
                            );

                            let render_task_id = render_tasks.add(blur_render_task);

                            surfaces[surface_index.0].tasks.push(render_task_id);

                            render_task_id
                        }
                    );

                    PictureSurface::TextureCache(cache_item)
                }
            }
            PictureCompositeMode::Filter(FilterOp::DropShadow(offset, blur_radius, color)) => {
                let blur_std_deviation = blur_radius * frame_context.device_pixel_scale.0;
                let blur_range = (blur_std_deviation * BLUR_SAMPLE_SCALE).ceil() as i32;

                // The clipped field is the part of the picture that is visible
                // on screen. The unclipped field is the screen-space rect of
                // the complete picture, if no screen / clip-chain was applied
                // (this includes the extra space for blur region). To ensure
                // that we draw a large enough part of the picture to get correct
                // blur results, inflate that clipped area by the blur range, and
                // then intersect with the total screen rect, to minimize the
                // allocation size.
                let device_rect = clipped
                    .inflate(blur_range, blur_range)
                    .intersection(&unclipped.to_i32())
                    .unwrap();

                let uv_rect_kind = calculate_uv_rect_kind(
                    &pic_rect,
                    &transform,
                    &device_rect,
                    frame_context.device_pixel_scale,
                );

                let mut picture_task = RenderTask::new_picture(
                    RenderTaskLocation::Dynamic(None, device_rect.size),
                    unclipped.size,
                    pic_index,
                    device_rect.origin,
                    child_tasks,
                    uv_rect_kind,
                    pic_context.raster_spatial_node_index,
                    None,
                    Vec::new(),
                );
                picture_task.mark_for_saving();

                let picture_task_id = frame_state.render_tasks.add(picture_task);

                let blur_render_task = RenderTask::new_blur(
                    blur_std_deviation.round(),
                    picture_task_id,
                    frame_state.render_tasks,
                    RenderTargetKind::Color,
                    ClearMode::Transparent,
                );

                self.secondary_render_task_id = Some(picture_task_id);

                let render_task_id = frame_state.render_tasks.add(blur_render_task);
                surfaces[surface_index.0].tasks.push(render_task_id);

                if let Some(mut request) = frame_state.gpu_cache.request(&mut self.extra_gpu_data_handle) {
                    // TODO(gw): This is very hacky code below! It stores an extra
                    //           brush primitive below for the special case of a
                    //           drop-shadow where we need a different local
                    //           rect for the shadow. To tidy this up in future,
                    //           we could consider abstracting the code in prim_store.rs
                    //           that writes a brush primitive header.

                    // Basic brush primitive header is (see end of prepare_prim_for_render_inner in prim_store.rs)
                    //  [brush specific data]
                    //  [segment_rect, segment data]
                    let shadow_rect = prim_local_rect.translate(&offset);

                    // ImageBrush colors
                    request.push(color.premultiplied());
                    request.push(PremultipliedColorF::WHITE);
                    request.push([
                        prim_local_rect.size.width,
                        prim_local_rect.size.height,
                        0.0,
                        0.0,
                    ]);

                    // segment rect / extra data
                    request.push(shadow_rect);
                    request.push([0.0, 0.0, 0.0, 0.0]);
                }

                PictureSurface::RenderTask(render_task_id)
            }
            PictureCompositeMode::MixBlend(..) => {
                let uv_rect_kind = calculate_uv_rect_kind(
                    &pic_rect,
                    &transform,
                    &clipped,
                    frame_context.device_pixel_scale,
                );

                let picture_task = RenderTask::new_picture(
                    RenderTaskLocation::Dynamic(None, clipped.size),
                    unclipped.size,
                    pic_index,
                    clipped.origin,
                    child_tasks,
                    uv_rect_kind,
                    pic_context.raster_spatial_node_index,
                    None,
                    Vec::new(),
                );

                let readback_task_id = frame_state.render_tasks.add(
                    RenderTask::new_readback(clipped)
                );

                self.secondary_render_task_id = Some(readback_task_id);
                surfaces[surface_index.0].tasks.push(readback_task_id);

                let render_task_id = frame_state.render_tasks.add(picture_task);
                surfaces[surface_index.0].tasks.push(render_task_id);
                PictureSurface::RenderTask(render_task_id)
            }
            PictureCompositeMode::Filter(filter) => {
                if let FilterOp::ColorMatrix(m) = filter {
                    if let Some(mut request) = frame_state.gpu_cache.request(&mut self.extra_gpu_data_handle) {
                        for i in 0..5 {
                            request.push([m[i*4], m[i*4+1], m[i*4+2], m[i*4+3]]);
                        }
                    }
                }

                let uv_rect_kind = calculate_uv_rect_kind(
                    &pic_rect,
                    &transform,
                    &clipped,
                    frame_context.device_pixel_scale,
                );

                let picture_task = RenderTask::new_picture(
                    RenderTaskLocation::Dynamic(None, clipped.size),
                    unclipped.size,
                    pic_index,
                    clipped.origin,
                    child_tasks,
                    uv_rect_kind,
                    pic_context.raster_spatial_node_index,
                    None,
                    Vec::new(),
                );

                let render_task_id = frame_state.render_tasks.add(picture_task);
                surfaces[surface_index.0].tasks.push(render_task_id);
                PictureSurface::RenderTask(render_task_id)
            }
            PictureCompositeMode::Blit => {
                let uv_rect_kind = calculate_uv_rect_kind(
                    &pic_rect,
                    &transform,
                    &clipped,
                    frame_context.device_pixel_scale,
                );

                let picture_task = RenderTask::new_picture(
                    RenderTaskLocation::Dynamic(None, clipped.size),
                    unclipped.size,
                    pic_index,
                    clipped.origin,
                    child_tasks,
                    uv_rect_kind,
                    pic_context.raster_spatial_node_index,
                    None,
                    Vec::new(),
                );

                let render_task_id = frame_state.render_tasks.add(picture_task);
                surfaces[surface_index.0].tasks.push(render_task_id);
                PictureSurface::RenderTask(render_task_id)
            }
        };

        surfaces[raster_config.surface_index.0].surface = Some(surface);

        true
    }
}

// Calculate a single screen-space UV for a picture.
fn calculate_screen_uv(
    local_pos: &PicturePoint,
    transform: &PictureToRasterTransform,
    rendered_rect: &DeviceRect,
    device_pixel_scale: DevicePixelScale,
) -> DevicePoint {
    let raster_pos = match transform.transform_point2d(local_pos) {
        Some(pos) => pos,
        None => {
            //Warning: this is incorrect and needs to be fixed properly.
            // The transformation has put a local vertex behind the near clipping plane...
            // Proper solution would be to keep the near-clipping-plane results around
            // (currently produced by calculate_screen_bounding_rect) and use them here.
            return DevicePoint::new(0.5, 0.5);
        }
    };

    let raster_to_device_space = TypedScale::new(1.0) * device_pixel_scale;

    let mut device_pos = raster_pos * raster_to_device_space;

    // Apply snapping for axis-aligned scroll nodes, as per prim_shared.glsl.
    if transform.transform_kind() == TransformedRectKind::AxisAligned {
        device_pos.x = (device_pos.x + 0.5).floor();
        device_pos.y = (device_pos.y + 0.5).floor();
    }

    DevicePoint::new(
        (device_pos.x - rendered_rect.origin.x) / rendered_rect.size.width,
        (device_pos.y - rendered_rect.origin.y) / rendered_rect.size.height,
    )
}

// Calculate a UV rect within an image based on the screen space
// vertex positions of a picture.
fn calculate_uv_rect_kind(
    pic_rect: &PictureRect,
    transform: &PictureToRasterTransform,
    rendered_rect: &DeviceIntRect,
    device_pixel_scale: DevicePixelScale,
) -> UvRectKind {
    let rendered_rect = rendered_rect.to_f32();

    let top_left = calculate_screen_uv(
        &pic_rect.origin,
        transform,
        &rendered_rect,
        device_pixel_scale,
    );

    let top_right = calculate_screen_uv(
        &pic_rect.top_right(),
        transform,
        &rendered_rect,
        device_pixel_scale,
    );

    let bottom_left = calculate_screen_uv(
        &pic_rect.bottom_left(),
        transform,
        &rendered_rect,
        device_pixel_scale,
    );

    let bottom_right = calculate_screen_uv(
        &pic_rect.bottom_right(),
        transform,
        &rendered_rect,
        device_pixel_scale,
    );

    UvRectKind::Quad {
        top_left,
        top_right,
        bottom_left,
        bottom_right,
    }
}

fn create_raster_mappers(
    surface_spatial_node_index: SpatialNodeIndex,
    raster_spatial_node_index: SpatialNodeIndex,
    dirty_world_rect: WorldRect,
    clip_scroll_tree: &ClipScrollTree,
) -> (SpaceMapper<RasterPixel, WorldPixel>, SpaceMapper<PicturePixel, RasterPixel>) {
    let map_raster_to_world = SpaceMapper::new_with_target(
        ROOT_SPATIAL_NODE_INDEX,
        raster_spatial_node_index,
        dirty_world_rect,
        clip_scroll_tree,
    );

    let raster_bounds = map_raster_to_world.unmap(&dirty_world_rect)
                                           .unwrap_or(RasterRect::max_rect());

    let map_pic_to_raster = SpaceMapper::new_with_target(
        raster_spatial_node_index,
        surface_spatial_node_index,
        raster_bounds,
        clip_scroll_tree,
    );

    (map_raster_to_world, map_pic_to_raster)
}

// Check whether a relative transform between two spatial nodes has changed
// since last frame. If that relative transform hasn't been calculated, then
// do that now and store it for later use.
fn get_global_transform_changed(
    global_transforms: &mut [GlobalTransformInfo],
    spatial_node_index: SpatialNodeIndex,
    clip_scroll_tree: &ClipScrollTree,
    surface_spatial_node_index: SpatialNodeIndex,
) -> bool {
    let transform = &mut global_transforms[spatial_node_index.0];

    if transform.current.is_none() {
        let mapping: CoordinateSpaceMapping<LayoutPixel, PicturePixel> = CoordinateSpaceMapping::new(
            surface_spatial_node_index,
            spatial_node_index,
            clip_scroll_tree,
        ).expect("todo: handle invalid mappings");

        transform.current = Some(mapping.into());
        transform.changed = true;
    }

    transform.changed
}
||||||| merged common ancestors
=======
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use api::{MixBlendMode, PipelineId, PremultipliedColorF};
use api::{PropertyBinding, PropertyBindingId, FontRenderMode};
use api::{DebugFlags, RasterSpace, ImageKey, ColorF};
use api::units::*;
use crate::box_shadow::{BLUR_SAMPLE_SCALE};
use crate::clip::{ClipStore, ClipDataStore, ClipChainInstance};
use crate::clip_scroll_tree::{ROOT_SPATIAL_NODE_INDEX,
    ClipScrollTree, CoordinateSpaceMapping, SpatialNodeIndex, VisibleFace, CoordinateSystemId
};
use crate::debug_colors;
use euclid::{vec3, TypedPoint2D, TypedScale, TypedSize2D, Vector2D, TypedRect};
use euclid::approxeq::ApproxEq;
use crate::frame_builder::{FrameVisibilityContext, FrameVisibilityState};
use crate::intern::ItemUid;
use crate::internal_types::{FastHashMap, FastHashSet, PlaneSplitter, Filter};
use crate::frame_builder::{FrameBuildingContext, FrameBuildingState, PictureState, PictureContext};
use crate::gpu_cache::{GpuCache, GpuCacheAddress, GpuCacheHandle};
use crate::gpu_types::UvRectKind;
use plane_split::{Clipper, Polygon, Splitter};
use crate::prim_store::{SpaceMapper, PrimitiveVisibilityMask, PointKey, PrimitiveTemplateKind};
use crate::prim_store::{PictureIndex, PrimitiveInstance, PrimitiveInstanceKind};
use crate::prim_store::{get_raster_rects, PrimitiveScratchBuffer, RectangleKey};
use crate::prim_store::{OpacityBindingStorage, ImageInstanceStorage, OpacityBindingIndex};
use crate::print_tree::PrintTreePrinter;
use crate::render_backend::DataStores;
use crate::render_task::{ClearMode, RenderTask};
use crate::render_task::{RenderTaskId, RenderTaskLocation, BlurTaskCache};
use crate::resource_cache::ResourceCache;
use crate::scene::SceneProperties;
use crate::scene_builder::Interners;
use crate::spatial_node::SpatialNodeType;
use smallvec::SmallVec;
use std::{mem, u16};
use std::sync::atomic::{AtomicUsize, Ordering};
use crate::texture_cache::TextureCacheHandle;
use crate::tiling::RenderTargetKind;
use crate::util::{ComparableVec, TransformedRectKind, MatrixHelpers, MaxRect, scale_factors};
use crate::filterdata::{FilterDataHandle};

/*
 A picture represents a dynamically rendered image. It consists of:

 * A number of primitives that are drawn onto the picture.
 * A composite operation describing how to composite this
   picture into its parent.
 * A configuration describing how to draw the primitives on
   this picture (e.g. in screen space or local space).
 */

/// Specify whether a surface allows subpixel AA text rendering.
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum SubpixelMode {
    /// This surface allows subpixel AA text
    Allow,
    /// Subpixel AA text cannot be drawn on this surface
    Deny,
}

/// A comparable transform matrix, that compares with epsilon checks.
#[derive(Debug, Clone)]
struct MatrixKey {
    m: [f32; 16],
}

impl PartialEq for MatrixKey {
    fn eq(&self, other: &Self) -> bool {
        const EPSILON: f32 = 0.001;

        // TODO(gw): It's possible that we may need to adjust the epsilon
        //           to be tighter on most of the matrix, except the
        //           translation parts?
        for (i, j) in self.m.iter().zip(other.m.iter()) {
            if !i.approx_eq_eps(j, &EPSILON) {
                return false;
            }
        }

        true
    }
}

/// A comparable / hashable version of a coordinate space mapping. Used to determine
/// if a transform dependency for a tile has changed.
#[derive(Debug, PartialEq, Clone)]
enum TransformKey {
    Local,
    ScaleOffset {
        scale_x: f32,
        scale_y: f32,
        offset_x: f32,
        offset_y: f32,
    },
    Transform {
        m: MatrixKey,
    }
}

impl<Src, Dst> From<CoordinateSpaceMapping<Src, Dst>> for TransformKey {
    fn from(transform: CoordinateSpaceMapping<Src, Dst>) -> TransformKey {
        match transform {
            CoordinateSpaceMapping::Local => {
                TransformKey::Local
            }
            CoordinateSpaceMapping::ScaleOffset(ref scale_offset) => {
                TransformKey::ScaleOffset {
                    scale_x: scale_offset.scale.x,
                    scale_y: scale_offset.scale.y,
                    offset_x: scale_offset.offset.x,
                    offset_y: scale_offset.offset.y,
                }
            }
            CoordinateSpaceMapping::Transform(ref m) => {
                TransformKey::Transform {
                    m: MatrixKey {
                        m: m.to_row_major_array(),
                    },
                }
            }
        }
    }
}

/// Information about a picture that is pushed / popped on the
/// PictureUpdateState during picture traversal pass.
struct PictureInfo {
    /// The spatial node for this picture.
    _spatial_node_index: SpatialNodeIndex,
}

pub struct PictureCacheState {
    /// The tiles retained by this picture cache.
    pub tiles: FastHashMap<TileOffset, Tile>,
    /// The current fractional offset of the cache transform root.
    fract_offset: PictureVector2D,
}

/// Stores a list of cached picture tiles that are retained
/// between new scenes.
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct RetainedTiles {
    /// The tiles retained between display lists.
    #[cfg_attr(feature = "capture", serde(skip))] //TODO
    pub caches: FastHashMap<usize, PictureCacheState>,
}

impl RetainedTiles {
    pub fn new() -> Self {
        RetainedTiles {
            caches: FastHashMap::default(),
        }
    }

    /// Merge items from one retained tiles into another.
    pub fn merge(&mut self, other: RetainedTiles) {
        assert!(self.caches.is_empty() || other.caches.is_empty());
        if self.caches.is_empty() {
            self.caches = other.caches;
        }
    }
}

/// Unit for tile coordinates.
#[derive(Hash, Clone, Copy, Debug, Eq, PartialEq, Ord, PartialOrd)]
pub struct TileCoordinate;

// Geometry types for tile coordinates.
pub type TileOffset = TypedPoint2D<i32, TileCoordinate>;
pub type TileSize = TypedSize2D<i32, TileCoordinate>;
pub type TileRect = TypedRect<i32, TileCoordinate>;

/// The size in device pixels of a cached tile. The currently chosen
/// size is arbitrary. We should do some profiling to find the best
/// size for real world pages.
///
/// Note that we use a separate, smaller size during wrench testing, so that
/// we get tighter dirty rects and can do more meaningful invalidation
/// tests.
pub const TILE_SIZE_WIDTH: i32 = 1024;
pub const TILE_SIZE_HEIGHT: i32 = 512;

/// The maximum size per axis of a surface,
///  in WorldPixel coordinates.
const MAX_SURFACE_SIZE: f32 = 4096.0;

/// Used to get unique tile IDs, even when the tile cache is
/// destroyed between display lists / scenes.
static NEXT_TILE_ID: AtomicUsize = AtomicUsize::new(0);

fn clamp(value: i32, low: i32, high: i32) -> i32 {
    value.max(low).min(high)
}

fn clampf(value: f32, low: f32, high: f32) -> f32 {
    value.max(low).min(high)
}

/// Information about the state of an opacity binding.
#[derive(Debug)]
pub struct OpacityBindingInfo {
    /// The current value retrieved from dynamic scene properties.
    value: f32,
    /// True if it was changed (or is new) since the last frame build.
    changed: bool,
}

/// Information stored in a tile descriptor for an opacity binding.
#[derive(Debug, PartialEq, Clone)]
pub enum OpacityBinding {
    Value(f32),
    Binding(PropertyBindingId),
}

impl From<PropertyBinding<f32>> for OpacityBinding {
    fn from(binding: PropertyBinding<f32>) -> OpacityBinding {
        match binding {
            PropertyBinding::Binding(key, _) => OpacityBinding::Binding(key.id),
            PropertyBinding::Value(value) => OpacityBinding::Value(value),
        }
    }
}

/// A stable ID for a given tile, to help debugging.
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct TileId(usize);

/// Information about a cached tile.
#[derive(Debug)]
pub struct Tile {
    /// The current world rect of this tile.
    pub world_rect: WorldRect,
    /// The current local rect of this tile.
    pub rect: PictureRect,
    /// The local rect of the tile clipped to the overal picture local rect.
    clipped_rect: PictureRect,
    /// Uniquely describes the content of this tile, in a way that can be
    /// (reasonably) efficiently hashed and compared.
    pub descriptor: TileDescriptor,
    /// Handle to the cached texture for this tile.
    pub handle: TextureCacheHandle,
    /// If true, this tile is marked valid, and the existing texture
    /// cache handle can be used. Tiles are invalidated during the
    /// build_dirty_regions method.
    pub is_valid: bool,
    /// If true, the content on this tile is the same as last frame.
    is_same_content: bool,
    /// The tile id is stable between display lists and / or frames,
    /// if the tile is retained. Useful for debugging tile evictions.
    pub id: TileId,
    /// The set of transforms that affect primitives on this tile we
    /// care about. Stored as a set here, and then collected, sorted
    /// and converted to transform key values during post_update.
    transforms: FastHashSet<SpatialNodeIndex>,
    /// Bitfield specifying the dirty region(s) that are relevant to this tile.
    visibility_mask: PrimitiveVisibilityMask,
    /// If true, the tile was determined to be opaque, which means blending
    /// can be disabled when drawing it.
    pub is_opaque: bool,
}

impl Tile {
    /// Construct a new, invalid tile.
    fn new(
        id: TileId,
    ) -> Self {
        Tile {
            rect: PictureRect::zero(),
            clipped_rect: PictureRect::zero(),
            world_rect: WorldRect::zero(),
            handle: TextureCacheHandle::invalid(),
            descriptor: TileDescriptor::new(),
            is_same_content: false,
            is_valid: false,
            transforms: FastHashSet::default(),
            id,
            visibility_mask: PrimitiveVisibilityMask::empty(),
            is_opaque: false,
        }
    }

    /// Clear the dependencies for a tile.
    fn clear(&mut self) {
        self.transforms.clear();
        self.descriptor.clear();
    }

    /// Invalidate a tile based on change in content. This
    /// must be called even if the tile is not currently
    /// visible on screen. We might be able to improve this
    /// later by changing how ComparableVec is used.
    fn update_content_validity(&mut self) {
        // Check if the contents of the primitives, clips, and
        // other dependencies are the same.
        self.is_same_content &= self.descriptor.is_same_content(self.id);
        self.is_valid &= self.is_same_content;
    }
}

/// Defines a key that uniquely identifies a primitive instance.
#[derive(Debug, Clone)]
pub struct PrimitiveDescriptor {
    /// Uniquely identifies the content of the primitive template.
    prim_uid: ItemUid,
    /// The origin in world space of this primitive.
    origin: PointKey,
    /// The clip rect for this primitive. Included here in
    /// dependencies since there is no entry in the clip chain
    /// dependencies for the local clip rect.
    prim_clip_rect: RectangleKey,
    /// The first clip in the clip_uids array of clips that affect this tile.
    first_clip: u16,
    /// The number of clips that affect this primitive instance.
    clip_count: u16,
}

impl PartialEq for PrimitiveDescriptor {
    fn eq(&self, other: &Self) -> bool {
        const EPSILON: f32 = 0.001;

        if self.prim_uid != other.prim_uid {
            return false;
        }
        if self.first_clip != other.first_clip {
            return false;
        }
        if self.clip_count != other.clip_count {
            return false;
        }

        if !self.origin.x.approx_eq_eps(&other.origin.x, &EPSILON) {
            return false;
        }
        if !self.origin.y.approx_eq_eps(&other.origin.y, &EPSILON) {
            return false;
        }

        if !self.prim_clip_rect.x.approx_eq_eps(&other.prim_clip_rect.x, &EPSILON) {
            return false;
        }
        if !self.prim_clip_rect.y.approx_eq_eps(&other.prim_clip_rect.y, &EPSILON) {
            return false;
        }
        if !self.prim_clip_rect.w.approx_eq_eps(&other.prim_clip_rect.w, &EPSILON) {
            return false;
        }
        if !self.prim_clip_rect.h.approx_eq_eps(&other.prim_clip_rect.h, &EPSILON) {
            return false;
        }

        true
    }
}

/// Defines a key that uniquely identifies a clip instance.
#[derive(Debug, Clone)]
pub struct ClipDescriptor {
    /// The uid is guaranteed to uniquely describe the content of the clip node.
    uid: ItemUid,
    /// The origin defines the relative position of this clip template.
    origin: PointKey,
}

impl PartialEq for ClipDescriptor {
    fn eq(&self, other: &Self) -> bool {
        const EPSILON: f32 = 0.001;

        if self.uid != other.uid {
            return false;
        }

        if !self.origin.x.approx_eq_eps(&other.origin.x, &EPSILON) {
            return false;
        }

        if !self.origin.y.approx_eq_eps(&other.origin.y, &EPSILON) {
            return false;
        }

        true
    }
}

/// Uniquely describes the content of this tile, in a way that can be
/// (reasonably) efficiently hashed and compared.
#[derive(Debug)]
pub struct TileDescriptor {
    /// List of primitive instance unique identifiers. The uid is guaranteed
    /// to uniquely describe the content of the primitive template, while
    /// the other parameters describe the clip chain and instance params.
    pub prims: ComparableVec<PrimitiveDescriptor>,

    /// List of clip node descriptors.
    clips: ComparableVec<ClipDescriptor>,

    /// List of image keys that this tile depends on.
    image_keys: ComparableVec<ImageKey>,

    /// The set of opacity bindings that this tile depends on.
    // TODO(gw): Ugh, get rid of all opacity binding support!
    opacity_bindings: ComparableVec<OpacityBinding>,

    /// List of the effects of transforms that we care about
    /// tracking for this tile.
    transforms: ComparableVec<TransformKey>,
}

impl TileDescriptor {
    fn new() -> Self {
        TileDescriptor {
            prims: ComparableVec::new(),
            clips: ComparableVec::new(),
            opacity_bindings: ComparableVec::new(),
            image_keys: ComparableVec::new(),
            transforms: ComparableVec::new(),
        }
    }

    /// Clear the dependency information for a tile, when the dependencies
    /// are being rebuilt.
    fn clear(&mut self) {
        self.prims.reset();
        self.clips.reset();
        self.opacity_bindings.reset();
        self.image_keys.reset();
        self.transforms.reset();
    }

    /// Return true if the content of the tile is the same
    /// as last frame. This doesn't check validity of the
    /// tile based on the currently valid regions.
    fn is_same_content(&self, _id: TileId) -> bool {
        if !self.image_keys.is_valid() {
            return false;
        }
        if !self.opacity_bindings.is_valid() {
            return false;
        }
        if !self.clips.is_valid() {
            return false;
        }
        if !self.prims.is_valid() {
            return false;
        }
        if !self.transforms.is_valid() {
            return false;
        }

        true
    }
}

/// Stores both the world and devices rects for a single dirty rect.
#[derive(Debug, Clone)]
pub struct DirtyRegionRect {
    /// World rect of this dirty region
    pub world_rect: WorldRect,
    /// Bitfield for picture render tasks that draw this dirty region.
    pub visibility_mask: PrimitiveVisibilityMask,
}

/// Represents the dirty region of a tile cache picture.
#[derive(Debug, Clone)]
pub struct DirtyRegion {
    /// The individual dirty rects of this region.
    pub dirty_rects: Vec<DirtyRegionRect>,

    /// The overall dirty rect, a combination of dirty_rects
    pub combined: WorldRect,
}

impl DirtyRegion {
    /// Construct a new dirty region tracker.
    pub fn new(
    ) -> Self {
        DirtyRegion {
            dirty_rects: Vec::with_capacity(PrimitiveVisibilityMask::MAX_DIRTY_REGIONS),
            combined: WorldRect::zero(),
        }
    }

    /// Reset the dirty regions back to empty
    pub fn clear(&mut self) {
        self.dirty_rects.clear();
        self.combined = WorldRect::zero();
    }

    /// Push a dirty rect into this region
    pub fn push(
        &mut self,
        rect: WorldRect,
        visibility_mask: PrimitiveVisibilityMask,
    ) {
        // Include this in the overall dirty rect
        self.combined = self.combined.union(&rect);

        // Store the individual dirty rect.
        self.dirty_rects.push(DirtyRegionRect {
            world_rect: rect,
            visibility_mask,
        });
    }

    /// Include another rect into an existing dirty region.
    pub fn include_rect(
        &mut self,
        region_index: usize,
        rect: WorldRect,
    ) {
        self.combined = self.combined.union(&rect);

        let region = &mut self.dirty_rects[region_index];
        region.world_rect = region.world_rect.union(&rect);
    }

    // TODO(gw): This returns a heap allocated object. Perhaps we can simplify this
    //           logic? Although - it's only used very rarely so it may not be an issue.
    pub fn inflate(
        &self,
        inflate_amount: f32,
    ) -> DirtyRegion {
        let mut dirty_rects = Vec::with_capacity(self.dirty_rects.len());
        let mut combined = WorldRect::zero();

        for rect in &self.dirty_rects {
            let world_rect = rect.world_rect.inflate(inflate_amount, inflate_amount);
            combined = combined.union(&world_rect);
            dirty_rects.push(DirtyRegionRect {
                world_rect,
                visibility_mask: rect.visibility_mask,
            });
        }

        DirtyRegion {
            dirty_rects,
            combined,
        }
    }

    /// Creates a record of this dirty region for exporting to test infrastructure.
    pub fn record(&self) -> RecordedDirtyRegion {
        let mut rects: Vec<WorldRect> =
            self.dirty_rects.iter().map(|r| r.world_rect.clone()).collect();
        rects.sort_unstable_by_key(|r| (r.origin.y as usize, r.origin.x as usize));
        RecordedDirtyRegion { rects }
    }
}

/// A recorded copy of the dirty region for exporting to test infrastructure.
pub struct RecordedDirtyRegion {
    pub rects: Vec<WorldRect>,
}

impl ::std::fmt::Display for RecordedDirtyRegion {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        for r in self.rects.iter() {
            let (x, y, w, h) = (r.origin.x, r.origin.y, r.size.width, r.size.height);
            write!(f, "[({},{}):{}x{}]", x, y, w, h)?;
        }
        Ok(())
    }
}

impl ::std::fmt::Debug for RecordedDirtyRegion {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::std::fmt::Display::fmt(self, f)
    }
}

/// Represents a cache of tiles that make up a picture primitives.
pub struct TileCacheInstance {
    /// Index of the tile cache / slice for this frame builder. It's determined
    /// by the setup_picture_caching method during flattening, which splits the
    /// picture tree into multiple slices. It's used as a simple input to the tile
    /// keys. It does mean we invalidate tiles if a new layer gets inserted / removed
    /// between display lists - this seems very unlikely to occur on most pages, but
    /// can be revisited if we ever notice that.
    pub slice: usize,
    /// The positioning node for this tile cache.
    pub spatial_node_index: SpatialNodeIndex,
    /// Hash of tiles present in this picture.
    pub tiles: FastHashMap<TileOffset, Tile>,
    /// A helper struct to map local rects into surface coords.
    map_local_to_surface: SpaceMapper<LayoutPixel, PicturePixel>,
    /// List of opacity bindings, with some extra information
    /// about whether they changed since last frame.
    opacity_bindings: FastHashMap<PropertyBindingId, OpacityBindingInfo>,
    /// The current dirty region tracker for this picture.
    pub dirty_region: DirtyRegion,
    /// Current size of tiles in picture units.
    tile_size: PictureSize,
    /// Tile coords of the currently allocated grid.
    tile_rect: TileRect,
    /// Pre-calculated versions of the tile_rect above, used to speed up the
    /// calculations in get_tile_coords_for_rect.
    tile_bounds_p0: TileOffset,
    tile_bounds_p1: TileOffset,
    /// Local rect (unclipped) of the picture this cache covers.
    pub local_rect: PictureRect,
    /// Local clip rect for this tile cache.
    pub local_clip_rect: PictureRect,
    /// A list of tiles that are valid and visible, which should be drawn to the main scene.
    pub tiles_to_draw: Vec<TileOffset>,
    /// The world space viewport that this tile cache draws into.
    /// Any clips outside this viewport can be ignored (and must be removed so that
    /// we can draw outside the bounds of the viewport).
    pub world_viewport_rect: WorldRect,
    /// The surface index that this tile cache will be drawn into.
    surface_index: SurfaceIndex,
    /// The background color from the renderer. If this is set opaque, we know it's
    /// fine to clear the tiles to this and allow subpixel text on the first slice.
    pub background_color: Option<ColorF>,
    /// The picture space rectangle that is known to be opaque. This is used
    /// to determine where subpixel AA can be used, and where alpha blending
    /// can be disabled.
    pub opaque_rect: PictureRect,
    /// The allowed subpixel mode for this surface, which depends on the detected
    /// opacity of the background.
    pub subpixel_mode: SubpixelMode,
    /// The current fractional offset of the cache transform root. If this changes,
    /// all tiles need to be invalidated and redrawn, since snapping differences are
    /// likely to occur.
    fract_offset: PictureVector2D,
}

impl TileCacheInstance {
    pub fn new(
        slice: usize,
        spatial_node_index: SpatialNodeIndex,
        background_color: Option<ColorF>,
    ) -> Self {
        TileCacheInstance {
            slice,
            spatial_node_index,
            tiles: FastHashMap::default(),
            map_local_to_surface: SpaceMapper::new(
                ROOT_SPATIAL_NODE_INDEX,
                PictureRect::zero(),
            ),
            opacity_bindings: FastHashMap::default(),
            dirty_region: DirtyRegion::new(),
            tile_size: PictureSize::zero(),
            tile_rect: TileRect::zero(),
            tile_bounds_p0: TileOffset::zero(),
            tile_bounds_p1: TileOffset::zero(),
            local_rect: PictureRect::zero(),
            local_clip_rect: PictureRect::zero(),
            tiles_to_draw: Vec::new(),
            world_viewport_rect: WorldRect::zero(),
            surface_index: SurfaceIndex(0),
            background_color,
            opaque_rect: PictureRect::zero(),
            subpixel_mode: SubpixelMode::Allow,
            fract_offset: PictureVector2D::zero(),
        }
    }

    /// Returns true if this tile cache is considered opaque.
    pub fn is_opaque(&self) -> bool {
        // If known opaque due to background clear color and being the first slice.
        // The background_color will only be Some(..) if this is the first slice.
        match self.background_color {
            Some(color) => color.a >= 1.0,
            None => false
        }
    }

    /// Get the tile coordinates for a given rectangle.
    fn get_tile_coords_for_rect(
        &self,
        rect: &PictureRect,
    ) -> (TileOffset, TileOffset) {
        // Get the tile coordinates in the picture space.
        let mut p0 = TileOffset::new(
            (rect.origin.x / self.tile_size.width).floor() as i32,
            (rect.origin.y / self.tile_size.height).floor() as i32,
        );

        let mut p1 = TileOffset::new(
            ((rect.origin.x + rect.size.width) / self.tile_size.width).ceil() as i32,
            ((rect.origin.y + rect.size.height) / self.tile_size.height).ceil() as i32,
        );

        // Clamp the tile coordinates here to avoid looping over irrelevant tiles later on.
        p0.x = clamp(p0.x, self.tile_bounds_p0.x, self.tile_bounds_p1.x);
        p0.y = clamp(p0.y, self.tile_bounds_p0.y, self.tile_bounds_p1.y);
        p1.x = clamp(p1.x, self.tile_bounds_p0.x, self.tile_bounds_p1.x);
        p1.y = clamp(p1.y, self.tile_bounds_p0.y, self.tile_bounds_p1.y);

        (p0, p1)
    }

    /// Update transforms, opacity bindings and tile rects.
    pub fn pre_update(
        &mut self,
        pic_rect: PictureRect,
        surface_index: SurfaceIndex,
        frame_context: &FrameVisibilityContext,
        frame_state: &mut FrameVisibilityState,
    ) -> WorldRect {
        let tile_width = TILE_SIZE_WIDTH;
        let tile_height = TILE_SIZE_HEIGHT;
        self.surface_index = surface_index;

        // Reset the opaque rect + subpixel mode, as they are calculated
        // during the prim dependency checks.
        self.opaque_rect = PictureRect::zero();
        self.subpixel_mode = SubpixelMode::Allow;

        self.map_local_to_surface = SpaceMapper::new(
            self.spatial_node_index,
            PictureRect::from_untyped(&pic_rect.to_untyped()),
        );

        let pic_to_world_mapper = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            self.spatial_node_index,
            frame_context.global_screen_world_rect,
            frame_context.clip_scroll_tree,
        );

        // If there are pending retained state, retrieve it.
        if let Some(prev_state) = frame_state.retained_tiles.caches.remove(&self.slice) {
            self.tiles.extend(prev_state.tiles);
            self.fract_offset = prev_state.fract_offset;
        }

        // Map an arbitrary point in picture space to world space, to work out
        // what the fractional translation is that's applied by this scroll root.
        // TODO(gw): I'm not 100% sure this is right. At least, in future, we should
        //           make a specific API for this, and/or enforce that the picture
        //           cache transform only includes scale and/or translation (we
        //           already ensure it doesn't have perspective).
        let world_origin = pic_to_world_mapper
            .map(&PictureRect::new(PicturePoint::zero(), PictureSize::new(1.0, 1.0)))
            .expect("bug: unable to map origin to world space")
            .origin;

        // Get the desired integer device coordinate
        let device_origin = world_origin * frame_context.global_device_pixel_scale;
        let desired_device_origin = device_origin.round();

        // Unmap from device space to world space rect
        let ref_world_rect = WorldRect::new(
            desired_device_origin / frame_context.global_device_pixel_scale,
            WorldSize::new(1.0, 1.0),
        );

        // Unmap from world space to picture space
        let ref_point = pic_to_world_mapper
            .unmap(&ref_world_rect)
            .expect("bug: unable to unmap ref world rect")
            .origin;

        // Extract the fractional offset required in picture space to align in device space
        let fract_offset = PictureVector2D::new(
            ref_point.x.fract(),
            ref_point.y.fract(),
        );

        // Determine if the fractional offset of the transform is different this frame
        // from the currently cached tile set.
        let fract_changed = (self.fract_offset.x - fract_offset.x).abs() > 0.001 ||
                            (self.fract_offset.y - fract_offset.y).abs() > 0.001;
        if fract_changed {
            self.fract_offset = fract_offset;
        }

        let spatial_node = &frame_context
            .clip_scroll_tree
            .spatial_nodes[self.spatial_node_index.0 as usize];
        let (viewport_rect, viewport_spatial_node_index) = match spatial_node.node_type {
            SpatialNodeType::ScrollFrame(ref info) => {
                (info.viewport_rect, spatial_node.parent.unwrap())
            }
            SpatialNodeType::StickyFrame(..) => {
                unreachable!();
            }
            SpatialNodeType::ReferenceFrame(..) => {
                assert_eq!(self.spatial_node_index, ROOT_SPATIAL_NODE_INDEX);
                (LayoutRect::max_rect(), ROOT_SPATIAL_NODE_INDEX)
            }
        };

        let viewport_to_world_mapper = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            viewport_spatial_node_index,
            frame_context.global_screen_world_rect,
            frame_context.clip_scroll_tree,
        );
        self.world_viewport_rect = viewport_to_world_mapper
            .map(&viewport_rect)
            .expect("bug: unable to map viewport to world space");

        // TODO(gw): This is a reverse mapping. It should always work since we know
        //           that this path only runs for slices in the root coordinate system.
        //           But perhaps we should assert that?
        // TODO(gw): We could change to directly use the ScaleOffset in content_transform
        //           which would make this clearer that we know the coordinate systems are the
        //           same and that it's a safe / exact conversion.
        self.map_local_to_surface.set_target_spatial_node(
            viewport_spatial_node_index,
            frame_context.clip_scroll_tree,
        );
        let local_viewport_rect = self
            .map_local_to_surface
            .map(&viewport_rect)
            .expect("bug: unable to map to local viewport rect");

        self.local_rect = pic_rect;
        self.local_clip_rect = local_viewport_rect;

        // Do a hacky diff of opacity binding values from the last frame. This is
        // used later on during tile invalidation tests.
        let current_properties = frame_context.scene_properties.float_properties();
        let old_properties = mem::replace(&mut self.opacity_bindings, FastHashMap::default());

        for (id, value) in current_properties {
            let changed = match old_properties.get(id) {
                Some(old_property) => !old_property.value.approx_eq(value),
                None => true,
            };
            self.opacity_bindings.insert(*id, OpacityBindingInfo {
                value: *value,
                changed,
            });
        }

        let world_tile_size = WorldSize::new(
            tile_width as f32 / frame_context.global_device_pixel_scale.0,
            tile_height as f32 / frame_context.global_device_pixel_scale.0,
        );

        // We know that this is an exact rectangle, since we (for now) only support tile
        // caches where the scroll root is in the root coordinate system.
        let local_tile_rect = pic_to_world_mapper
            .unmap(&WorldRect::new(WorldPoint::zero(), world_tile_size))
            .expect("bug: unable to get local tile rect");

        self.tile_size = local_tile_rect.size;

        let screen_rect_in_pic_space = pic_to_world_mapper
            .unmap(&frame_context.global_screen_world_rect)
            .expect("unable to unmap screen rect");

        let visible_rect_in_pic_space = screen_rect_in_pic_space
            .intersection(&self.local_clip_rect)
            .unwrap_or(PictureRect::zero());

        // Inflate the needed rect a bit, so that we retain tiles that we have drawn
        // but have just recently gone off-screen. This means that we avoid re-drawing
        // tiles if the user is scrolling up and down small amounts, at the cost of
        // a bit of extra texture memory.
        let desired_rect_in_pic_space = visible_rect_in_pic_space
            .inflate(0.0, 3.0 * self.tile_size.height);

        let needed_rect_in_pic_space = desired_rect_in_pic_space
            .intersection(&pic_rect)
            .unwrap_or(PictureRect::zero());

        let p0 = needed_rect_in_pic_space.origin;
        let p1 = needed_rect_in_pic_space.bottom_right();

        let x0 = (p0.x / local_tile_rect.size.width).floor() as i32;
        let x1 = (p1.x / local_tile_rect.size.width).ceil() as i32;

        let y0 = (p0.y / local_tile_rect.size.height).floor() as i32;
        let y1 = (p1.y / local_tile_rect.size.height).ceil() as i32;

        let x_tiles = x1 - x0;
        let y_tiles = y1 - y0;
        self.tile_rect = TileRect::new(
            TileOffset::new(x0, y0),
            TileSize::new(x_tiles, y_tiles),
        );
        // This is duplicated information from tile_rect, but cached here to avoid
        // redundant calculations during get_tile_coords_for_rect
        self.tile_bounds_p0 = TileOffset::new(x0, y0);
        self.tile_bounds_p1 = TileOffset::new(x1, y1);

        let mut world_culling_rect = WorldRect::zero();

        let mut old_tiles = mem::replace(
            &mut self.tiles,
            FastHashMap::default(),
        );

        for y in y0 .. y1 {
            for x in x0 .. x1 {
                let key = TileOffset::new(x, y);

                let mut tile = old_tiles
                    .remove(&key)
                    .unwrap_or_else(|| {
                        let next_id = TileId(NEXT_TILE_ID.fetch_add(1, Ordering::Relaxed));
                        Tile::new(next_id)
                    });

                // Ensure each tile is offset by the appropriate amount from the
                // origin, such that the content origin will be a whole number and
                // the snapping will be consistent.
                tile.rect = PictureRect::new(
                    PicturePoint::new(
                        x as f32 * self.tile_size.width + fract_offset.x,
                        y as f32 * self.tile_size.height + fract_offset.y,
                    ),
                    self.tile_size,
                );

                tile.clipped_rect = tile.rect
                    .intersection(&self.local_rect)
                    .unwrap_or(PictureRect::zero());

                tile.world_rect = pic_to_world_mapper
                    .map(&tile.rect)
                    .expect("bug: map local tile rect");

                world_culling_rect = world_culling_rect.union(&tile.world_rect);

                self.tiles.insert(key, tile);
            }
        }

        // Do tile invalidation for any dependencies that we know now.
        for (_, tile) in &mut self.tiles {
            // Start frame assuming that the tile has the same content,
            // unless the fractional offset of the transform root changed.
            tile.is_same_content = !fract_changed;

            // Content has changed if any opacity bindings changed.
            for binding in tile.descriptor.opacity_bindings.items() {
                if let OpacityBinding::Binding(id) = binding {
                    let changed = match self.opacity_bindings.get(id) {
                        Some(info) => info.changed,
                        None => true,
                    };
                    if changed {
                        tile.is_same_content = false;
                        break;
                    }
                }
            }

            // Clear any dependencies so that when we rebuild them we
            // can compare if the tile has the same content.
            tile.clear();
        }

        world_culling_rect
    }

    /// Update the dependencies for each tile for a given primitive instance.
    pub fn update_prim_dependencies(
        &mut self,
        prim_instance: &PrimitiveInstance,
        prim_clip_chain: Option<&ClipChainInstance>,
        local_prim_rect: LayoutRect,
        clip_scroll_tree: &ClipScrollTree,
        data_stores: &DataStores,
        clip_store: &ClipStore,
        pictures: &[PicturePrimitive],
        resource_cache: &ResourceCache,
        opacity_binding_store: &OpacityBindingStorage,
        image_instances: &ImageInstanceStorage,
        surface_index: SurfaceIndex,
    ) -> bool {
        self.map_local_to_surface.set_target_spatial_node(
            prim_instance.spatial_node_index,
            clip_scroll_tree,
        );

        // Map the primitive local rect into picture space.
        let prim_rect = match self.map_local_to_surface.map(&local_prim_rect) {
            Some(rect) => rect,
            None => return false,
        };

        // If the rect is invalid, no need to create dependencies.
        if prim_rect.size.is_empty_or_negative() {
            return false;
        }

        // Get the tile coordinates in the picture space.
        let (p0, p1) = self.get_tile_coords_for_rect(&prim_rect);

        // If the primitive is outside the tiling rects, it's known to not
        // be visible.
        if p0.x == p1.x || p0.y == p1.y {
            return false;
        }

        // Build the list of resources that this primitive has dependencies on.
        let mut opacity_bindings: SmallVec<[OpacityBinding; 4]> = SmallVec::new();
        let mut clips: SmallVec<[ClipDescriptor; 8]> = SmallVec::new();
        let mut image_keys: SmallVec<[ImageKey; 8]> = SmallVec::new();
        let mut clip_spatial_nodes = FastHashSet::default();
        let mut prim_clip_rect = PictureRect::zero();

        // Some primitives can not be cached (e.g. external video images)
        let is_cacheable = prim_instance.is_cacheable(
            &data_stores,
            resource_cache,
        );

        // If there was a clip chain, add any clip dependencies to the list for this tile.
        if let Some(prim_clip_chain) = prim_clip_chain {
            prim_clip_rect = prim_clip_chain.pic_clip_rect;

            let clip_instances = &clip_store
                .clip_node_instances[prim_clip_chain.clips_range.to_range()];
            for clip_instance in clip_instances {
                clips.push(ClipDescriptor {
                    uid: clip_instance.handle.uid(),
                    origin: clip_instance.local_pos.into(),
                });

                // If the clip has the same spatial node, the relative transform
                // will always be the same, so there's no need to depend on it.
                if clip_instance.spatial_node_index != self.spatial_node_index {
                    clip_spatial_nodes.insert(clip_instance.spatial_node_index);
                }
            }
        }

        // For pictures, we don't (yet) know the valid clip rect, so we can't correctly
        // use it to calculate the local bounding rect for the tiles. If we include them
        // then we may calculate a bounding rect that is too large, since it won't include
        // the clip bounds of the picture. Excluding them from the bounding rect here
        // fixes any correctness issues (the clips themselves are considered when we
        // consider the bounds of the primitives that are *children* of the picture),
        // however it does potentially result in some un-necessary invalidations of a
        // tile (in cases where the picture local rect affects the tile, but the clip
        // rect eventually means it doesn't affect that tile).
        // TODO(gw): Get picture clips earlier (during the initial picture traversal
        //           pass) so that we can calculate these correctly.
        let clip_by_tile = match prim_instance.kind {
            PrimitiveInstanceKind::Picture { pic_index,.. } => {
                // Pictures can depend on animated opacity bindings.
                let pic = &pictures[pic_index.0];
                if let Some(PictureCompositeMode::Filter(Filter::Opacity(binding, _))) = pic.requested_composite_mode {
                    opacity_bindings.push(binding.into());
                }

                false
            }
            PrimitiveInstanceKind::Rectangle { data_handle, opacity_binding_index, .. } => {
                if opacity_binding_index == OpacityBindingIndex::INVALID {
                    // Check a number of conditions to see if we can consider this
                    // primitive as an opaque rect. Several of these are conservative
                    // checks and could be relaxed in future. However, these checks
                    // are quick and capture the common cases of background rects.
                    // Specifically, we currently require:
                    //  - No opacity binding (to avoid resolving the opacity here).
                    //  - Color.a >= 1.0 (the primitive is opaque).
                    //  - Same coord system as picture cache (ensures rects are axis-aligned).
                    //  - No clip masks exist.

                    let on_picture_surface = surface_index == self.surface_index;

                    let prim_is_opaque = match data_stores.prim[data_handle].kind {
                        PrimitiveTemplateKind::Rectangle { ref color, .. } => color.a >= 1.0,
                        _ => unreachable!(),
                    };

                    let same_coord_system = {
                        let prim_spatial_node = &clip_scroll_tree
                            .spatial_nodes[prim_instance.spatial_node_index.0 as usize];
                        let surface_spatial_node = &clip_scroll_tree
                            .spatial_nodes[self.spatial_node_index.0 as usize];

                        prim_spatial_node.coordinate_system_id == surface_spatial_node.coordinate_system_id
                    };

                    if let Some(ref clip_chain) = prim_clip_chain {
                        if prim_is_opaque && same_coord_system && !clip_chain.needs_mask && on_picture_surface {
                            if clip_chain.pic_clip_rect.contains_rect(&self.opaque_rect) {
                                self.opaque_rect = clip_chain.pic_clip_rect;
                            }
                        }
                    };
                } else {
                    let opacity_binding = &opacity_binding_store[opacity_binding_index];
                    for binding in &opacity_binding.bindings {
                        opacity_bindings.push(OpacityBinding::from(*binding));
                    }
                }

                true
            }
            PrimitiveInstanceKind::Image { data_handle, image_instance_index, .. } => {
                let image_data = &data_stores.image[data_handle].kind;
                let image_instance = &image_instances[image_instance_index];
                let opacity_binding_index = image_instance.opacity_binding_index;

                if opacity_binding_index != OpacityBindingIndex::INVALID {
                    let opacity_binding = &opacity_binding_store[opacity_binding_index];
                    for binding in &opacity_binding.bindings {
                        opacity_bindings.push(OpacityBinding::from(*binding));
                    }
                }

                image_keys.push(image_data.key);
                false
            }
            PrimitiveInstanceKind::YuvImage { data_handle, .. } => {
                let yuv_image_data = &data_stores.yuv_image[data_handle].kind;
                image_keys.extend_from_slice(&yuv_image_data.yuv_key);
                false
            }
            PrimitiveInstanceKind::PushClipChain |
            PrimitiveInstanceKind::PopClipChain => {
                // Early exit to ensure this doesn't get added as a dependency on the tile.
                return false;
            }
            PrimitiveInstanceKind::TextRun { data_handle, .. } => {
                // Only do these checks if we haven't already disabled subpx
                // text rendering for this slice.
                if self.subpixel_mode == SubpixelMode::Allow && !self.is_opaque() {
                    let run_data = &data_stores.text_run[data_handle];

                    // If a text run is on a child surface, the subpx mode will be
                    // correctly determined as we recurse through pictures in take_context.
                    let on_picture_surface = surface_index == self.surface_index;

                    // Only care about text runs that have requested subpixel rendering.
                    // This is conservative - it may still end up that a subpx requested
                    // text run doesn't get subpx for other reasons (e.g. glyph size).
                    let subpx_requested = match run_data.font.render_mode {
                        FontRenderMode::Subpixel => true,
                        FontRenderMode::Alpha | FontRenderMode::Mono => false,
                    };

                    if on_picture_surface && subpx_requested {
                        if !self.opaque_rect.contains_rect(&prim_clip_rect) {
                            self.subpixel_mode = SubpixelMode::Deny;
                        }
                    }
                }

                false
            }
            PrimitiveInstanceKind::LineDecoration { .. } |
            PrimitiveInstanceKind::Clear { .. } |
            PrimitiveInstanceKind::NormalBorder { .. } |
            PrimitiveInstanceKind::LinearGradient { .. } |
            PrimitiveInstanceKind::RadialGradient { .. } |
            PrimitiveInstanceKind::ImageBorder { .. } => {
                // These don't contribute dependencies
                false
            }
        };

        // Normalize the tile coordinates before adding to tile dependencies.
        // For each affected tile, mark any of the primitive dependencies.
        for y in p0.y .. p1.y {
            for x in p0.x .. p1.x {
                // TODO(gw): Convert to 2d array temporarily to avoid hash lookups per-tile?
                let key = TileOffset::new(x, y);
                let tile = self.tiles.get_mut(&key).expect("bug: no tile");

                // Mark if the tile is cacheable at all.
                tile.is_same_content &= is_cacheable;

                // Include any image keys this tile depends on.
                tile.descriptor.image_keys.extend_from_slice(&image_keys);

                // // Include any opacity bindings this primitive depends on.
                tile.descriptor.opacity_bindings.extend_from_slice(&opacity_bindings);

                // TODO(gw): The origin of background rects produced by APZ changes
                //           in Gecko during scrolling. Consider investigating this so the
                //           hack / workaround below is not required.
                let (prim_origin, prim_clip_rect) = if clip_by_tile {
                    let tile_p0 = tile.clipped_rect.origin;
                    let tile_p1 = tile.clipped_rect.bottom_right();

                    let clip_p0 = PicturePoint::new(
                        clampf(prim_clip_rect.origin.x, tile_p0.x, tile_p1.x),
                        clampf(prim_clip_rect.origin.y, tile_p0.y, tile_p1.y),
                    );

                    let clip_p1 = PicturePoint::new(
                        clampf(prim_clip_rect.origin.x + prim_clip_rect.size.width, tile_p0.x, tile_p1.x),
                        clampf(prim_clip_rect.origin.y + prim_clip_rect.size.height, tile_p0.y, tile_p1.y),
                    );

                    (
                        PicturePoint::new(
                            clampf(prim_rect.origin.x, tile_p0.x, tile_p1.x),
                            clampf(prim_rect.origin.y, tile_p0.y, tile_p1.y),
                        ),
                        PictureRect::new(
                            clip_p0,
                            PictureSize::new(
                                clip_p1.x - clip_p0.x,
                                clip_p1.y - clip_p0.y,
                            ),
                        ),
                    )
                } else {
                    (prim_rect.origin, prim_clip_rect)
                };

                // Update the tile descriptor, used for tile comparison during scene swaps.
                tile.descriptor.prims.push(PrimitiveDescriptor {
                    prim_uid: prim_instance.uid(),
                    origin: prim_origin.into(),
                    first_clip: tile.descriptor.clips.len() as u16,
                    clip_count: clips.len() as u16,
                    prim_clip_rect: prim_clip_rect.into(),
                });

                tile.descriptor.clips.extend_from_slice(&clips);

                // If the primitive has the same spatial node, the relative transform
                // will always be the same, so there's no need to depend on it.
                if prim_instance.spatial_node_index != self.spatial_node_index {
                    tile.transforms.insert(prim_instance.spatial_node_index);
                }
                for spatial_node_index in &clip_spatial_nodes {
                    tile.transforms.insert(*spatial_node_index);
                }
            }
        }

        true
    }

    /// Apply any updates after prim dependency updates. This applies
    /// any late tile invalidations, and sets up the dirty rect and
    /// set of tile blits.
    pub fn post_update(
        &mut self,
        resource_cache: &mut ResourceCache,
        gpu_cache: &mut GpuCache,
        frame_context: &FrameVisibilityContext,
        scratch: &mut PrimitiveScratchBuffer,
    ) {
        self.tiles_to_draw.clear();
        self.dirty_region.clear();
        let mut dirty_region_index = 0;

        // Step through each tile and invalidate if the dependencies have changed.
        for (key, tile) in self.tiles.iter_mut() {
            // Check if this tile can be considered opaque.
            tile.is_opaque = self.opaque_rect.contains_rect(&tile.clipped_rect);

            // Update tile transforms
            let mut transform_spatial_nodes: Vec<SpatialNodeIndex> = tile.transforms.drain().collect();
            transform_spatial_nodes.sort();
            for spatial_node_index in transform_spatial_nodes {
                // Note: this is the only place where we don't know beforehand if the tile-affecting
                // spatial node is below or above the current picture.
                let transform = if self.spatial_node_index >= spatial_node_index {
                    frame_context.clip_scroll_tree
                        .get_relative_transform(
                            self.spatial_node_index,
                            spatial_node_index,
                        )
                } else {
                    frame_context.clip_scroll_tree
                        .get_relative_transform(
                            spatial_node_index,
                            self.spatial_node_index,
                        )
                };
                tile.descriptor.transforms.push(transform.into());
            }

            // Content has changed if any images have changed.
            // NOTE: This invalidation must be done after the request_resources
            //       calls for primitives during visibility update, or the
            //       is_image_dirty check may be incorrect.
            for image_key in tile.descriptor.image_keys.items() {
                if resource_cache.is_image_dirty(*image_key) {
                    tile.is_same_content = false;
                    break;
                }
            }

            // Invalidate if the backing texture was evicted.
            if resource_cache.texture_cache.is_allocated(&tile.handle) {
                // Request the backing texture so it won't get evicted this frame.
                // We specifically want to mark the tile texture as used, even
                // if it's detected not visible below and skipped. This is because
                // we maintain the set of tiles we care about based on visibility
                // during pre_update. If a tile still exists after that, we are
                // assuming that it's either visible or we want to retain it for
                // a while in case it gets scrolled back onto screen soon.
                // TODO(gw): Consider switching to manual eviction policy?
                resource_cache.texture_cache.request(&tile.handle, gpu_cache);
            } else {
                // When a tile is invalidated, reset the opacity information
                // so that it is recalculated during prim dependency updates.
                tile.is_valid = false;
            }

            // Invalidate the tile based on the content changing.
            tile.update_content_validity();

            // If there are no primitives there is no need to draw or cache it.
            if tile.descriptor.prims.is_empty() {
                continue;
            }

            if !tile.world_rect.intersects(&frame_context.global_screen_world_rect) {
                continue;
            }

            self.tiles_to_draw.push(*key);

            // Decide how to handle this tile when drawing this frame.
            if tile.is_valid {
                if frame_context.debug_flags.contains(DebugFlags::PICTURE_CACHING_DBG) {
                    let tile_device_rect = tile.world_rect * frame_context.global_device_pixel_scale;
                    let label_offset = DeviceVector2D::new(20.0, 30.0);
                    let color = if tile.is_opaque {
                        debug_colors::GREEN
                    } else {
                        debug_colors::YELLOW
                    };
                    scratch.push_debug_rect(
                        tile_device_rect,
                        color.scale_alpha(0.3),
                    );
                    if tile_device_rect.size.height >= label_offset.y {
                        scratch.push_debug_string(
                            tile_device_rect.origin + label_offset,
                            debug_colors::RED,
                            format!("{:?}: is_opaque={}", tile.id, tile.is_opaque),
                        );
                    }
                }
            } else {
                if frame_context.debug_flags.contains(DebugFlags::PICTURE_CACHING_DBG) {
                    scratch.push_debug_rect(
                        tile.world_rect * frame_context.global_device_pixel_scale,
                        debug_colors::RED,
                    );
                }

                // Ensure that this texture is allocated.
                if !resource_cache.texture_cache.is_allocated(&tile.handle) {
                    let tile_size = DeviceIntSize::new(
                        TILE_SIZE_WIDTH,
                        TILE_SIZE_HEIGHT,
                    );
                    resource_cache.texture_cache.update_picture_cache(
                        tile_size,
                        &mut tile.handle,
                        gpu_cache,
                    );
                }

                tile.visibility_mask = PrimitiveVisibilityMask::empty();

                // If we run out of dirty regions, then force the last dirty region to
                // be a union of any remaining regions. This is an inefficiency, in that
                // we'll add items to batches later on that are redundant / outside this
                // tile, but it's really rare except in pathological cases (even on a
                // 4k screen, the typical dirty region count is < 16).
                if dirty_region_index < PrimitiveVisibilityMask::MAX_DIRTY_REGIONS {
                    tile.visibility_mask.set_visible(dirty_region_index);

                    self.dirty_region.push(
                        tile.world_rect,
                        tile.visibility_mask,
                    );

                    dirty_region_index += 1;
                } else {
                    tile.visibility_mask.set_visible(PrimitiveVisibilityMask::MAX_DIRTY_REGIONS - 1);

                    self.dirty_region.include_rect(
                        PrimitiveVisibilityMask::MAX_DIRTY_REGIONS - 1,
                        tile.world_rect,
                    );
                }
            }
        }

        // When under test, record a copy of the dirty region to support
        // invalidation testing in wrench.
        if frame_context.config.testing {
            scratch.recorded_dirty_regions.push(self.dirty_region.record());
        }
    }
}

/// Maintains a stack of picture and surface information, that
/// is used during the initial picture traversal.
pub struct PictureUpdateState<'a> {
    surfaces: &'a mut Vec<SurfaceInfo>,
    surface_stack: Vec<SurfaceIndex>,
    picture_stack: Vec<PictureInfo>,
    are_raster_roots_assigned: bool,
}

impl<'a> PictureUpdateState<'a> {
    pub fn update_all(
        surfaces: &'a mut Vec<SurfaceInfo>,
        pic_index: PictureIndex,
        picture_primitives: &mut [PicturePrimitive],
        frame_context: &FrameBuildingContext,
        gpu_cache: &mut GpuCache,
        clip_store: &ClipStore,
        clip_data_store: &ClipDataStore,
    ) {
        profile_marker!("UpdatePictures");

        let mut state = PictureUpdateState {
            surfaces,
            surface_stack: vec![SurfaceIndex(0)],
            picture_stack: Vec::new(),
            are_raster_roots_assigned: true,
        };

        state.update(
            pic_index,
            picture_primitives,
            frame_context,
            gpu_cache,
            clip_store,
            clip_data_store,
        );

        if !state.are_raster_roots_assigned {
            state.assign_raster_roots(
                pic_index,
                picture_primitives,
                ROOT_SPATIAL_NODE_INDEX,
            );
        }
    }

    /// Return the current surface
    fn current_surface(&self) -> &SurfaceInfo {
        &self.surfaces[self.surface_stack.last().unwrap().0]
    }

    /// Return the current surface (mutable)
    fn current_surface_mut(&mut self) -> &mut SurfaceInfo {
        &mut self.surfaces[self.surface_stack.last().unwrap().0]
    }

    /// Push a new surface onto the update stack.
    fn push_surface(
        &mut self,
        surface: SurfaceInfo,
    ) -> SurfaceIndex {
        let surface_index = SurfaceIndex(self.surfaces.len());
        self.surfaces.push(surface);
        self.surface_stack.push(surface_index);
        surface_index
    }

    /// Pop a surface on the way up the picture traversal
    fn pop_surface(&mut self) -> SurfaceIndex{
        self.surface_stack.pop().unwrap()
    }

    /// Push information about a picture on the update stack
    fn push_picture(
        &mut self,
        info: PictureInfo,
    ) {
        self.picture_stack.push(info);
    }

    /// Pop the picture info off, on the way up the picture traversal
    fn pop_picture(
        &mut self,
    ) -> PictureInfo {
        self.picture_stack.pop().unwrap()
    }

    /// Update a picture, determining surface configuration,
    /// rasterization roots, and (in future) whether there
    /// are cached surfaces that can be used by this picture.
    fn update(
        &mut self,
        pic_index: PictureIndex,
        picture_primitives: &mut [PicturePrimitive],
        frame_context: &FrameBuildingContext,
        gpu_cache: &mut GpuCache,
        clip_store: &ClipStore,
        clip_data_store: &ClipDataStore,
    ) {
        if let Some(prim_list) = picture_primitives[pic_index.0].pre_update(
            self,
            frame_context,
        ) {
            for child_pic_index in &prim_list.pictures {
                self.update(
                    *child_pic_index,
                    picture_primitives,
                    frame_context,
                    gpu_cache,
                    clip_store,
                    clip_data_store,
                );
            }

            picture_primitives[pic_index.0].post_update(
                prim_list,
                self,
                frame_context,
            );
        }
    }

    /// Process the picture tree again in a depth-first order,
    /// and adjust the raster roots of the pictures that want to establish
    /// their own roots but are not able to due to the size constraints.
    fn assign_raster_roots(
        &mut self,
        pic_index: PictureIndex,
        picture_primitives: &[PicturePrimitive],
        fallback_raster_spatial_node: SpatialNodeIndex,
    ) {
        let picture = &picture_primitives[pic_index.0];
        if !picture.is_visible() {
            return
        }

        let new_fallback = match picture.raster_config {
            Some(ref config) => {
                let surface = &mut self.surfaces[config.surface_index.0];
                if !config.establishes_raster_root {
                    surface.raster_spatial_node_index = fallback_raster_spatial_node;
                }
                surface.raster_spatial_node_index
            }
            None => fallback_raster_spatial_node,
        };

        for child_pic_index in &picture.prim_list.pictures {
            self.assign_raster_roots(*child_pic_index, picture_primitives, new_fallback);
        }
    }
}

#[derive(Debug, Copy, Clone, PartialEq)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct SurfaceIndex(pub usize);

pub const ROOT_SURFACE_INDEX: SurfaceIndex = SurfaceIndex(0);

#[derive(Debug, Copy, Clone)]
pub struct SurfaceRenderTasks {
    /// The root of the render task chain for this surface. This
    /// is attached to parent tasks, and also the surface that
    /// gets added during batching.
    pub root: RenderTaskId,
    /// The port of the render task change for this surface. This
    /// is where child tasks for this surface get attached to.
    pub port: RenderTaskId,
}

/// Information about an offscreen surface. For now,
/// it contains information about the size and coordinate
/// system of the surface. In the future, it will contain
/// information about the contents of the surface, which
/// will allow surfaces to be cached / retained between
/// frames and display lists.
#[derive(Debug)]
pub struct SurfaceInfo {
    /// A local rect defining the size of this surface, in the
    /// coordinate system of the surface itself.
    pub rect: PictureRect,
    /// Helper structs for mapping local rects in different
    /// coordinate systems into the surface coordinates.
    pub map_local_to_surface: SpaceMapper<LayoutPixel, PicturePixel>,
    /// Defines the positioning node for the surface itself,
    /// and the rasterization root for this surface.
    pub raster_spatial_node_index: SpatialNodeIndex,
    pub surface_spatial_node_index: SpatialNodeIndex,
    /// This is set when the render task is created.
    pub render_tasks: Option<SurfaceRenderTasks>,
    /// How much the local surface rect should be inflated (for blur radii).
    pub inflation_factor: f32,
    /// The device pixel ratio specific to this surface.
    pub device_pixel_scale: DevicePixelScale,
}

impl SurfaceInfo {
    pub fn new(
        surface_spatial_node_index: SpatialNodeIndex,
        raster_spatial_node_index: SpatialNodeIndex,
        inflation_factor: f32,
        world_rect: WorldRect,
        clip_scroll_tree: &ClipScrollTree,
        device_pixel_scale: DevicePixelScale,
    ) -> Self {
        let map_surface_to_world = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            world_rect,
            clip_scroll_tree,
        );

        let pic_bounds = map_surface_to_world
            .unmap(&map_surface_to_world.bounds)
            .unwrap_or_else(PictureRect::max_rect);

        let map_local_to_surface = SpaceMapper::new(
            surface_spatial_node_index,
            pic_bounds,
        );

        SurfaceInfo {
            rect: PictureRect::zero(),
            map_local_to_surface,
            render_tasks: None,
            raster_spatial_node_index,
            surface_spatial_node_index,
            inflation_factor,
            device_pixel_scale,
        }
    }
}

#[derive(Debug)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct RasterConfig {
    /// How this picture should be composited into
    /// the parent surface.
    pub composite_mode: PictureCompositeMode,
    /// Index to the surface descriptor for this
    /// picture.
    pub surface_index: SurfaceIndex,
    /// Whether this picture establishes a rasterization root.
    pub establishes_raster_root: bool,
}

bitflags! {
    /// A set of flags describing why a picture may need a backing surface.
    #[cfg_attr(feature = "capture", derive(Serialize))]
    pub struct BlitReason: u32 {
        /// Mix-blend-mode on a child that requires isolation.
        const ISOLATE = 1;
        /// Clip node that _might_ require a surface.
        const CLIP = 2;
        /// Preserve-3D requires a surface for plane-splitting.
        const PRESERVE3D = 4;
    }
}

/// Specifies how this Picture should be composited
/// onto the target it belongs to.
#[allow(dead_code)]
#[derive(Debug, Clone)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub enum PictureCompositeMode {
    /// Apply CSS mix-blend-mode effect.
    MixBlend(MixBlendMode),
    /// Apply a CSS filter (except component transfer).
    Filter(Filter),
    /// Apply a component transfer filter.
    ComponentTransferFilter(FilterDataHandle),
    /// Draw to intermediate surface, copy straight across. This
    /// is used for CSS isolation, and plane splitting.
    Blit(BlitReason),
    /// Used to cache a picture as a series of tiles.
    TileCache {
    },
}

/// Enum value describing the place of a picture in a 3D context.
#[derive(Clone, Debug)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub enum Picture3DContext<C> {
    /// The picture is not a part of 3D context sub-hierarchy.
    Out,
    /// The picture is a part of 3D context.
    In {
        /// Additional data per child for the case of this a root of 3D hierarchy.
        root_data: Option<Vec<C>>,
        /// The spatial node index of an "ancestor" element, i.e. one
        /// that establishes the transformed element’s containing block.
        ///
        /// See CSS spec draft for more details:
        /// https://drafts.csswg.org/css-transforms-2/#accumulated-3d-transformation-matrix-computation
        ancestor_index: SpatialNodeIndex,
    },
}

/// Information about a preserve-3D hierarchy child that has been plane-split
/// and ordered according to the view direction.
#[derive(Clone, Debug)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct OrderedPictureChild {
    pub anchor: usize,
    pub spatial_node_index: SpatialNodeIndex,
    pub gpu_address: GpuCacheAddress,
}

/// Defines the grouping key for a cluster of primitives in a picture.
/// In future this will also contain spatial grouping details.
#[derive(Debug, Hash, Eq, PartialEq, Copy, Clone)]
struct PrimitiveClusterKey {
    /// Grouping primitives by spatial node ensures that we can calculate a local
    /// bounding volume for the cluster, and then transform that by the spatial
    /// node transform once to get an updated bounding volume for the entire cluster.
    spatial_node_index: SpatialNodeIndex,
    /// We want to separate clusters that have different backface visibility properties
    /// so that we can accept / reject an entire cluster at once if the backface is not
    /// visible.
    is_backface_visible: bool,
}

/// Descriptor for a cluster of primitives. For now, this is quite basic but will be
/// extended to handle more spatial clustering of primitives.
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct PrimitiveCluster {
    /// The positioning node for this cluster.
    spatial_node_index: SpatialNodeIndex,
    /// Whether this cluster is visible when the position node is a backface.
    is_backface_visible: bool,
    /// The bounding rect of the cluster, in the local space of the spatial node.
    /// This is used to quickly determine the overall bounding rect for a picture
    /// during the first picture traversal, which is needed for local scale
    /// determination, and render task size calculations.
    bounding_rect: LayoutRect,
    /// This flag is set during the first pass picture traversal, depending on whether
    /// the cluster is visible or not. It's read during the second pass when primitives
    /// consult their owning clusters to see if the primitive itself is visible.
    pub is_visible: bool,
}

impl PrimitiveCluster {
    fn new(
        spatial_node_index: SpatialNodeIndex,
        is_backface_visible: bool,
    ) -> Self {
        PrimitiveCluster {
            bounding_rect: LayoutRect::zero(),
            spatial_node_index,
            is_backface_visible,
            is_visible: false,
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct PrimitiveClusterIndex(pub u32);

#[derive(Debug, Copy, Clone)]
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct ClusterIndex(pub u16);

impl ClusterIndex {
    pub const INVALID: ClusterIndex = ClusterIndex(u16::MAX);
}

/// A list of pictures, stored by the PrimitiveList to enable a
/// fast traversal of just the pictures.
pub type PictureList = SmallVec<[PictureIndex; 4]>;

/// A list of primitive instances that are added to a picture
/// This ensures we can keep a list of primitives that
/// are pictures, for a fast initial traversal of the picture
/// tree without walking the instance list.
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct PrimitiveList {
    /// The primitive instances, in render order.
    pub prim_instances: Vec<PrimitiveInstance>,
    /// List of pictures that are part of this list.
    /// Used to implement the picture traversal pass.
    pub pictures: PictureList,
    /// List of primitives grouped into clusters.
    pub clusters: SmallVec<[PrimitiveCluster; 4]>,
}

impl PrimitiveList {
    /// Construct an empty primitive list. This is
    /// just used during the take_context / restore_context
    /// borrow check dance, which will be removed as the
    /// picture traversal pass is completed.
    pub fn empty() -> Self {
        PrimitiveList {
            prim_instances: Vec::new(),
            pictures: SmallVec::new(),
            clusters: SmallVec::new(),
        }
    }

    /// Construct a new prim list from a list of instances
    /// in render order. This does some work during scene
    /// building which makes the frame building traversals
    /// significantly faster.
    pub fn new(
        mut prim_instances: Vec<PrimitiveInstance>,
        interners: &Interners
    ) -> Self {
        let mut pictures = SmallVec::new();
        let mut clusters_map = FastHashMap::default();
        let mut clusters: SmallVec<[PrimitiveCluster; 4]> = SmallVec::new();

        // Walk the list of primitive instances and extract any that
        // are pictures.
        for prim_instance in &mut prim_instances {
            // Check if this primitive is a picture. In future we should
            // remove this match and embed this info directly in the primitive instance.
            let is_pic = match prim_instance.kind {
                PrimitiveInstanceKind::Picture { pic_index, .. } => {
                    pictures.push(pic_index);
                    true
                }
                _ => {
                    false
                }
            };

            let (is_backface_visible, prim_size) = match prim_instance.kind {
                PrimitiveInstanceKind::Rectangle { data_handle, .. } |
                PrimitiveInstanceKind::Clear { data_handle, .. } => {
                    let data = &interners.prim[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::Image { data_handle, .. } => {
                    let data = &interners.image[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::ImageBorder { data_handle, .. } => {
                    let data = &interners.image_border[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::LineDecoration { data_handle, .. } => {
                    let data = &interners.line_decoration[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::LinearGradient { data_handle, .. } => {
                    let data = &interners.linear_grad[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::NormalBorder { data_handle, .. } => {
                    let data = &interners.normal_border[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::Picture { data_handle, .. } => {
                    let data = &interners.picture[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::RadialGradient { data_handle, ..} => {
                    let data = &interners.radial_grad[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::TextRun { data_handle, .. } => {
                    let data = &interners.text_run[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::YuvImage { data_handle, .. } => {
                    let data = &interners.yuv_image[data_handle];
                    (data.is_backface_visible, data.prim_size)
                }
                PrimitiveInstanceKind::PushClipChain |
                PrimitiveInstanceKind::PopClipChain => {
                    (true, LayoutSize::zero())
                }
            };

            // Get the key for the cluster that this primitive should
            // belong to.
            let key = PrimitiveClusterKey {
                spatial_node_index: prim_instance.spatial_node_index,
                is_backface_visible,
            };

            // Find the cluster, or create a new one.
            let cluster_index = *clusters_map
                .entry(key)
                .or_insert_with(|| {
                    let index = clusters.len();
                    clusters.push(PrimitiveCluster::new(
                        prim_instance.spatial_node_index,
                        is_backface_visible,
                    ));
                    index
                }
            );

            if prim_instance.is_chased() {
                println!("\tcluster {} with {:?}", cluster_index, key);
            }

            // Pictures don't have a known static local bounding rect (they are
            // calculated during the picture traversal dynamically). If not
            // a picture, include a minimal bounding rect in the cluster bounds.
            let cluster = &mut clusters[cluster_index];
            if !is_pic {
                let prim_rect = LayoutRect::new(
                    prim_instance.prim_origin,
                    prim_size,
                );
                let culling_rect = prim_instance.local_clip_rect
                    .intersection(&prim_rect)
                    .unwrap_or_else(LayoutRect::zero);

                cluster.bounding_rect = cluster.bounding_rect.union(&culling_rect);
            }

            prim_instance.cluster_index = ClusterIndex(cluster_index as u16);
        }

        PrimitiveList {
            prim_instances,
            pictures,
            clusters,
        }
    }
}

/// Defines configuration options for a given picture primitive.
#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct PictureOptions {
    /// If true, WR should inflate the bounding rect of primitives when
    /// using a filter effect that requires inflation.
    pub inflate_if_required: bool,
}

impl Default for PictureOptions {
    fn default() -> Self {
        PictureOptions {
            inflate_if_required: true,
        }
    }
}

#[cfg_attr(feature = "capture", derive(Serialize))]
pub struct PicturePrimitive {
    /// List of primitives, and associated info for this picture.
    pub prim_list: PrimitiveList,

    #[cfg_attr(feature = "capture", serde(skip))]
    pub state: Option<PictureState>,

    /// If true, apply the local clip rect to primitive drawn
    /// in this picture.
    pub apply_local_clip_rect: bool,
    /// If false and transform ends up showing the back of the picture,
    /// it will be considered invisible.
    pub is_backface_visible: bool,

    // If a mix-blend-mode, contains the render task for
    // the readback of the framebuffer that we use to sample
    // from in the mix-blend-mode shader.
    // For drop-shadow filter, this will store the original
    // picture task which would be rendered on screen after
    // blur pass.
    pub secondary_render_task_id: Option<RenderTaskId>,
    /// How this picture should be composited.
    /// If None, don't composite - just draw directly on parent surface.
    pub requested_composite_mode: Option<PictureCompositeMode>,
    /// Requested rasterization space for this picture. It is
    /// a performance hint only.
    pub requested_raster_space: RasterSpace,

    pub raster_config: Option<RasterConfig>,
    pub context_3d: Picture3DContext<OrderedPictureChild>,

    // If requested as a frame output (for rendering
    // pages to a texture), this is the pipeline this
    // picture is the root of.
    pub frame_output_pipeline_id: Option<PipelineId>,
    // Optional cache handles for storing extra data
    // in the GPU cache, depending on the type of
    // picture.
    pub extra_gpu_data_handles: SmallVec<[GpuCacheHandle; 1]>,

    /// The spatial node index of this picture when it is
    /// composited into the parent picture.
    pub spatial_node_index: SpatialNodeIndex,

    /// The local rect of this picture. It is built
    /// dynamically when updating visibility. It takes
    /// into account snapping in device space for its
    /// children.
    pub snapped_local_rect: LayoutRect,

    /// The local rect of this picture. It is built
    /// dynamically during the first picture traversal. It
    /// does not take into account snapping in device for
    /// its children.
    pub unsnapped_local_rect: LayoutRect,

    /// If false, this picture needs to (re)build segments
    /// if it supports segment rendering. This can occur
    /// if the local rect of the picture changes due to
    /// transform animation and/or scrolling.
    pub segments_are_valid: bool,

    /// If Some(..) the tile cache that is associated with this picture.
    #[cfg_attr(feature = "capture", serde(skip))] //TODO
    pub tile_cache: Option<Box<TileCacheInstance>>,

    /// The config options for this picture.
    options: PictureOptions,
}

impl PicturePrimitive {
    pub fn print<T: PrintTreePrinter>(
        &self,
        pictures: &[Self],
        self_index: PictureIndex,
        pt: &mut T,
    ) {
        pt.new_level(format!("{:?}", self_index));
        pt.add_item(format!("prim_count: {:?}", self.prim_list.prim_instances.len()));
        pt.add_item(format!("snapped_local_rect: {:?}", self.snapped_local_rect));
        pt.add_item(format!("unsnapped_local_rect: {:?}", self.unsnapped_local_rect));
        pt.add_item(format!("spatial_node_index: {:?}", self.spatial_node_index));
        pt.add_item(format!("raster_config: {:?}", self.raster_config));
        pt.add_item(format!("requested_composite_mode: {:?}", self.requested_composite_mode));

        for index in &self.prim_list.pictures {
            pictures[index.0].print(pictures, *index, pt);
        }

        pt.end_level();
    }

    /// Returns true if this picture supports segmented rendering.
    pub fn can_use_segments(&self) -> bool {
        match self.raster_config {
            // TODO(gw): Support brush segment rendering for filter and mix-blend
            //           shaders. It's possible this already works, but I'm just
            //           applying this optimization to Blit mode for now.
            Some(RasterConfig { composite_mode: PictureCompositeMode::MixBlend(..), .. }) |
            Some(RasterConfig { composite_mode: PictureCompositeMode::Filter(..), .. }) |
            Some(RasterConfig { composite_mode: PictureCompositeMode::ComponentTransferFilter(..), .. }) |
            Some(RasterConfig { composite_mode: PictureCompositeMode::TileCache { .. }, ..}) |
            None => {
                false
            }
            Some(RasterConfig { composite_mode: PictureCompositeMode::Blit(reason), ..}) => {
                reason == BlitReason::CLIP
            }
        }
    }

    fn resolve_scene_properties(&mut self, properties: &SceneProperties) -> bool {
        match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(ref mut filter)) => {
                match *filter {
                    Filter::Opacity(ref binding, ref mut value) => {
                        *value = properties.resolve_float(binding);
                    }
                    _ => {}
                }

                filter.is_visible()
            }
            _ => true,
        }
    }

    pub fn is_visible(&self) -> bool {
        match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(ref filter)) => {
                filter.is_visible()
            }
            _ => true,
        }
    }

    /// Destroy an existing picture. This is called just before
    /// a frame builder is replaced with a newly built scene. It
    /// gives a picture a chance to retain any cached tiles that
    /// may be useful during the next scene build.
    pub fn destroy(
        &mut self,
        retained_tiles: &mut RetainedTiles,
    ) {
        if let Some(tile_cache) = self.tile_cache.take() {
            if !tile_cache.tiles.is_empty() {
                retained_tiles.caches.insert(
                    tile_cache.slice,
                    PictureCacheState {
                        tiles: tile_cache.tiles,
                        fract_offset: tile_cache.fract_offset,
                    },
                );
            }
        }
    }

    // TODO(gw): We have the PictureOptions struct available. We
    //           should move some of the parameter list in this
    //           method to be part of the PictureOptions, and
    //           avoid adding new parameters here.
    pub fn new_image(
        requested_composite_mode: Option<PictureCompositeMode>,
        context_3d: Picture3DContext<OrderedPictureChild>,
        frame_output_pipeline_id: Option<PipelineId>,
        apply_local_clip_rect: bool,
        is_backface_visible: bool,
        requested_raster_space: RasterSpace,
        prim_list: PrimitiveList,
        spatial_node_index: SpatialNodeIndex,
        tile_cache: Option<Box<TileCacheInstance>>,
        options: PictureOptions,
    ) -> Self {
        PicturePrimitive {
            prim_list,
            state: None,
            secondary_render_task_id: None,
            requested_composite_mode,
            raster_config: None,
            context_3d,
            frame_output_pipeline_id,
            extra_gpu_data_handles: SmallVec::new(),
            apply_local_clip_rect,
            is_backface_visible,
            requested_raster_space,
            spatial_node_index,
            snapped_local_rect: LayoutRect::zero(),
            unsnapped_local_rect: LayoutRect::zero(),
            tile_cache,
            options,
            segments_are_valid: false,
        }
    }

    /// Gets the raster space to use when rendering the picture.
    /// Usually this would be the requested raster space. However, if the
    /// picture's spatial node or one of its ancestors is being pinch zoomed
    /// then we round it. This prevents us rasterizing glyphs for every minor
    /// change in zoom level, as that would be too expensive.
    pub fn get_raster_space(&self, clip_scroll_tree: &ClipScrollTree) -> RasterSpace {
        let spatial_node = &clip_scroll_tree.spatial_nodes[self.spatial_node_index.0 as usize];
        if spatial_node.is_ancestor_or_self_zooming {
            let scale_factors = clip_scroll_tree
                .get_relative_transform(self.spatial_node_index, ROOT_SPATIAL_NODE_INDEX)
                .scale_factors();

            // Round the scale up to the nearest power of 2, but don't exceed 8.
            let scale = scale_factors.0.max(scale_factors.1).min(8.0);
            let rounded_up = 1 << scale.log2().ceil() as u32;

            RasterSpace::Local(rounded_up as f32)
        } else {
            self.requested_raster_space
        }
    }

    pub fn take_context(
        &mut self,
        pic_index: PictureIndex,
        clipped_prim_bounding_rect: WorldRect,
        surface_spatial_node_index: SpatialNodeIndex,
        raster_spatial_node_index: SpatialNodeIndex,
        parent_surface_index: SurfaceIndex,
        parent_subpixel_mode: SubpixelMode,
        frame_state: &mut FrameBuildingState,
        frame_context: &FrameBuildingContext,
    ) -> Option<(PictureContext, PictureState, PrimitiveList)> {
        if !self.is_visible() {
            return None;
        }

        // Extract the raster and surface spatial nodes from the raster
        // config, if this picture establishes a surface. Otherwise just
        // pass in the spatial node indices from the parent context.
        let (raster_spatial_node_index, surface_spatial_node_index, surface_index, inflation_factor) = match self.raster_config {
            Some(ref raster_config) => {
                let surface = &frame_state.surfaces[raster_config.surface_index.0];

                (
                    surface.raster_spatial_node_index,
                    self.spatial_node_index,
                    raster_config.surface_index,
                    surface.inflation_factor,
                )
            }
            None => {
                (
                    raster_spatial_node_index,
                    surface_spatial_node_index,
                    parent_surface_index,
                    0.0,
                )
            }
        };

        let map_pic_to_world = SpaceMapper::new_with_target(
            ROOT_SPATIAL_NODE_INDEX,
            surface_spatial_node_index,
            frame_context.global_screen_world_rect,
            frame_context.clip_scroll_tree,
        );

        let pic_bounds = map_pic_to_world.unmap(&map_pic_to_world.bounds)
                                         .unwrap_or_else(PictureRect::max_rect);

        let map_local_to_pic = SpaceMapper::new(
            surface_spatial_node_index,
            pic_bounds,
        );

        let (map_raster_to_world, map_pic_to_raster) = create_raster_mappers(
            surface_spatial_node_index,
            raster_spatial_node_index,
            frame_context.global_screen_world_rect,
            frame_context.clip_scroll_tree,
        );

        let plane_splitter = match self.context_3d {
            Picture3DContext::Out => {
                None
            }
            Picture3DContext::In { root_data: Some(_), .. } => {
                Some(PlaneSplitter::new())
            }
            Picture3DContext::In { root_data: None, .. } => {
                None
            }
        };

        match self.raster_config {
            Some(ref raster_config) => {
                let pic_rect = PictureRect::from_untyped(&self.snapped_local_rect.to_untyped());

                let device_pixel_scale = frame_state
                    .surfaces[raster_config.surface_index.0]
                    .device_pixel_scale;

                let (clipped, unclipped) = match get_raster_rects(
                    pic_rect,
                    &map_pic_to_raster,
                    &map_raster_to_world,
                    clipped_prim_bounding_rect,
                    device_pixel_scale,
                ) {
                    Some(info) => info,
                    None => {
                        return None
                    }
                };
                let transform = map_pic_to_raster.get_transform();

                let dep_info = match raster_config.composite_mode {
                    PictureCompositeMode::Filter(Filter::Blur(blur_radius)) => {
                        let blur_std_deviation = blur_radius * device_pixel_scale.0;
                        let scale_factors = scale_factors(&transform);
                        let blur_std_deviation = DeviceSize::new(
                            blur_std_deviation * scale_factors.0,
                            blur_std_deviation * scale_factors.1
                        );
                        let inflation_factor = frame_state.surfaces[raster_config.surface_index.0].inflation_factor;
                        let inflation_factor = (inflation_factor * device_pixel_scale.0).ceil();

                        // The clipped field is the part of the picture that is visible
                        // on screen. The unclipped field is the screen-space rect of
                        // the complete picture, if no screen / clip-chain was applied
                        // (this includes the extra space for blur region). To ensure
                        // that we draw a large enough part of the picture to get correct
                        // blur results, inflate that clipped area by the blur range, and
                        // then intersect with the total screen rect, to minimize the
                        // allocation size.
                        // We cast clipped to f32 instead of casting unclipped to i32
                        // because unclipped can overflow an i32.
                        let device_rect = clipped.to_f32()
                            .inflate(inflation_factor, inflation_factor)
                            .intersection(&unclipped)
                            .unwrap();

                        let mut device_rect = match device_rect.try_cast::<i32>() {
                            Some(rect) => rect,
                            None => {
                                return None
                            }
                        };

                        // Adjust the size to avoid introducing sampling errors during the down-scaling passes.
                        // what would be even better is to rasterize the picture at the down-scaled size
                        // directly.
                        device_rect.size = RenderTask::adjusted_blur_source_size(
                            device_rect.size,
                            blur_std_deviation,
                        );

                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &device_rect,
                            device_pixel_scale,
                            true,
                        );

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, device_rect.size),
                            unclipped.size,
                            pic_index,
                            device_rect.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );

                        let picture_task_id = frame_state.render_tasks.add(picture_task);

                        let blur_render_task_id = RenderTask::new_blur(
                            blur_std_deviation,
                            picture_task_id,
                            frame_state.render_tasks,
                            RenderTargetKind::Color,
                            ClearMode::Transparent,
                            None,
                        );

                        Some((blur_render_task_id, picture_task_id))
                    }
                    PictureCompositeMode::Filter(Filter::DropShadows(ref shadows)) => {
                        let mut max_std_deviation = 0.0;
                        for shadow in shadows {
                            // TODO(nical) presumably we should compute the clipped rect for each shadow
                            // and compute the union of them to determine what we need to rasterize and blur?
                            max_std_deviation = f32::max(max_std_deviation, shadow.blur_radius * device_pixel_scale.0);
                        }

                        max_std_deviation = max_std_deviation.round();
                        let max_blur_range = (max_std_deviation * BLUR_SAMPLE_SCALE).ceil();
                        // We cast clipped to f32 instead of casting unclipped to i32
                        // because unclipped can overflow an i32.
                        let device_rect = clipped.to_f32()
                                .inflate(max_blur_range, max_blur_range)
                                .intersection(&unclipped)
                                .unwrap();

                        let mut device_rect = match device_rect.try_cast::<i32>() {
                            Some(rect) => rect,
                            None => {
                                return None
                            }
                        };

                        device_rect.size = RenderTask::adjusted_blur_source_size(
                            device_rect.size,
                            DeviceSize::new(max_std_deviation, max_std_deviation),
                        );

                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &device_rect,
                            device_pixel_scale,
                            true,
                        );

                        let mut picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, device_rect.size),
                            unclipped.size,
                            pic_index,
                            device_rect.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );
                        picture_task.mark_for_saving();

                        let picture_task_id = frame_state.render_tasks.add(picture_task);

                        self.secondary_render_task_id = Some(picture_task_id);

                        let mut blur_tasks = BlurTaskCache::default();

                        self.extra_gpu_data_handles.resize(shadows.len(), GpuCacheHandle::new());

                        let mut blur_render_task_id = picture_task_id;
                        for shadow in shadows {
                            let std_dev = f32::round(shadow.blur_radius * device_pixel_scale.0);
                            blur_render_task_id = RenderTask::new_blur(
                                DeviceSize::new(std_dev, std_dev),
                                picture_task_id,
                                frame_state.render_tasks,
                                RenderTargetKind::Color,
                                ClearMode::Transparent,
                                Some(&mut blur_tasks),
                            );
                        }

                        // TODO(nical) the second one should to be the blur's task id but we have several blurs now
                        Some((blur_render_task_id, picture_task_id))
                    }
                    PictureCompositeMode::MixBlend(..) if !frame_context.fb_config.gpu_supports_advanced_blend => {
                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &clipped,
                            device_pixel_scale,
                            true,
                        );

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, clipped.size),
                            unclipped.size,
                            pic_index,
                            clipped.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );

                        let readback_task_id = frame_state.render_tasks.add(
                            RenderTask::new_readback(clipped)
                        );

                        frame_state.render_tasks.add_dependency(
                            frame_state.surfaces[parent_surface_index.0].render_tasks.unwrap().port,
                            readback_task_id,
                        );

                        self.secondary_render_task_id = Some(readback_task_id);

                        let render_task_id = frame_state.render_tasks.add(picture_task);

                        Some((render_task_id, render_task_id))
                    }
                    PictureCompositeMode::Filter(..) => {
                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &clipped,
                            device_pixel_scale,
                            true,
                        );

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, clipped.size),
                            unclipped.size,
                            pic_index,
                            clipped.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );

                        let render_task_id = frame_state.render_tasks.add(picture_task);

                        Some((render_task_id, render_task_id))
                    }
                    PictureCompositeMode::ComponentTransferFilter(..) => {
                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &clipped,
                            device_pixel_scale,
                            true,
                        );

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, clipped.size),
                            unclipped.size,
                            pic_index,
                            clipped.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );

                        let render_task_id = frame_state.render_tasks.add(picture_task);

                        Some((render_task_id, render_task_id))
                    }
                    PictureCompositeMode::TileCache { .. } => {
                        let tile_cache = self.tile_cache.as_mut().unwrap();
                        let mut first = true;

                        let tile_size = DeviceSize::new(
                            TILE_SIZE_WIDTH as f32,
                            TILE_SIZE_HEIGHT as f32,
                        );

                        for key in &tile_cache.tiles_to_draw {
                            let tile = tile_cache.tiles.get_mut(key).expect("bug: no tile found!");

                            if tile.is_valid {
                                continue;
                            }

                            let content_origin_f = tile.world_rect.origin * device_pixel_scale;
                            let content_origin = content_origin_f.round();
                            debug_assert!((content_origin_f.x - content_origin.x).abs() < 0.01);
                            debug_assert!((content_origin_f.y - content_origin.y).abs() < 0.01);

                            let cache_item = frame_state.resource_cache.texture_cache.get(&tile.handle);

                            let task = RenderTask::new_picture(
                                RenderTaskLocation::PictureCache {
                                    texture: cache_item.texture_id,
                                    layer: cache_item.texture_layer,
                                    size: tile_size.to_i32(),
                                },
                                tile_size,
                                pic_index,
                                content_origin.to_i32(),
                                UvRectKind::Rect,
                                surface_spatial_node_index,
                                device_pixel_scale,
                                tile.visibility_mask,
                            );

                            let render_task_id = frame_state.render_tasks.add(task);

                            frame_state.render_tasks.add_dependency(
                                frame_state.surfaces[parent_surface_index.0].render_tasks.unwrap().port,
                                render_task_id,
                            );

                            if first {
                                // TODO(gw): Maybe we can restructure this code to avoid the
                                //           first hack here. Or at least explain it with a follow up
                                //           bug.
                                frame_state.surfaces[raster_config.surface_index.0].render_tasks = Some(SurfaceRenderTasks {
                                    root: render_task_id,
                                    port: render_task_id,
                                });

                                first = false;
                            }

                            tile.is_valid = true;
                        }

                        None
                    }
                    PictureCompositeMode::MixBlend(..) |
                    PictureCompositeMode::Blit(_) => {
                        // The SplitComposite shader used for 3d contexts doesn't snap
                        // to pixels, so we shouldn't snap our uv coordinates either.
                        let supports_snapping = match self.context_3d {
                            Picture3DContext::In{ .. } => false,
                            _ => true,
                        };

                        let uv_rect_kind = calculate_uv_rect_kind(
                            &pic_rect,
                            &transform,
                            &clipped,
                            device_pixel_scale,
                            supports_snapping,
                        );

                        let picture_task = RenderTask::new_picture(
                            RenderTaskLocation::Dynamic(None, clipped.size),
                            unclipped.size,
                            pic_index,
                            clipped.origin,
                            uv_rect_kind,
                            surface_spatial_node_index,
                            device_pixel_scale,
                            PrimitiveVisibilityMask::all(),
                        );

                        let render_task_id = frame_state.render_tasks.add(picture_task);

                        Some((render_task_id, render_task_id))
                    }
                };

                if let Some((root, port)) = dep_info {
                    frame_state.surfaces[raster_config.surface_index.0].render_tasks = Some(SurfaceRenderTasks {
                        root,
                        port,
                    });

                    frame_state.render_tasks.add_dependency(
                        frame_state.surfaces[parent_surface_index.0].render_tasks.unwrap().port,
                        root,
                    );
                }
            }
            None => {}
        };

        let state = PictureState {
            //TODO: check for MAX_CACHE_SIZE here?
            map_local_to_pic,
            map_pic_to_world,
            map_pic_to_raster,
            map_raster_to_world,
            plane_splitter,
        };

        let mut dirty_region_count = 0;

        // If this is a picture cache, push the dirty region to ensure any
        // child primitives are culled and clipped to the dirty rect(s).
        if let Some(RasterConfig { composite_mode: PictureCompositeMode::TileCache { .. }, .. }) = self.raster_config {
            let dirty_region = self.tile_cache.as_ref().unwrap().dirty_region.clone();
            frame_state.push_dirty_region(dirty_region);
            dirty_region_count += 1;
        }

        if inflation_factor > 0.0 {
            let inflated_region = frame_state.current_dirty_region().inflate(inflation_factor);
            frame_state.push_dirty_region(inflated_region);
            dirty_region_count += 1;
        }

        // Disallow subpixel AA if an intermediate surface is needed.
        // TODO(lsalzman): allow overriding parent if intermediate surface is opaque
        let (is_passthrough, subpixel_mode) = match self.raster_config {
            Some(RasterConfig { ref composite_mode, .. }) => {
                let subpixel_mode = match composite_mode {
                    PictureCompositeMode::TileCache { .. } => {
                        self.tile_cache.as_ref().unwrap().subpixel_mode
                    }
                    PictureCompositeMode::Blit(..) |
                    PictureCompositeMode::ComponentTransferFilter(..) |
                    PictureCompositeMode::Filter(..) |
                    PictureCompositeMode::MixBlend(..) => {
                        // TODO(gw): We can take advantage of the same logic that
                        //           exists in the opaque rect detection for tile
                        //           caches, to allow subpixel text on other surfaces
                        //           that can be detected as opaque.
                        SubpixelMode::Deny
                    }
                };

                (false, subpixel_mode)
            }
            None => {
                (true, SubpixelMode::Allow)
            }
        };

        // Still disable subpixel AA if parent forbids it
        let subpixel_mode = match (parent_subpixel_mode, subpixel_mode) {
            (SubpixelMode::Allow, SubpixelMode::Allow) => SubpixelMode::Allow,
            _ => SubpixelMode::Deny,
        };

        let context = PictureContext {
            pic_index,
            apply_local_clip_rect: self.apply_local_clip_rect,
            is_passthrough,
            raster_spatial_node_index,
            surface_spatial_node_index,
            surface_index,
            dirty_region_count,
            subpixel_mode,
        };

        let prim_list = mem::replace(&mut self.prim_list, PrimitiveList::empty());

        Some((context, state, prim_list))
    }

    pub fn restore_context(
        &mut self,
        prim_list: PrimitiveList,
        context: PictureContext,
        state: PictureState,
        frame_state: &mut FrameBuildingState,
    ) {
        // Pop any dirty regions this picture set
        for _ in 0 .. context.dirty_region_count {
            frame_state.pop_dirty_region();
        }

        self.prim_list = prim_list;
        self.state = Some(state);
    }

    pub fn take_state(&mut self) -> PictureState {
        self.state.take().expect("bug: no state present!")
    }

    /// Add a primitive instance to the plane splitter. The function would generate
    /// an appropriate polygon, clip it against the frustum, and register with the
    /// given plane splitter.
    pub fn add_split_plane(
        splitter: &mut PlaneSplitter,
        clip_scroll_tree: &ClipScrollTree,
        prim_spatial_node_index: SpatialNodeIndex,
        original_local_rect: LayoutRect,
        combined_local_clip_rect: &LayoutRect,
        world_rect: WorldRect,
        plane_split_anchor: usize,
    ) -> bool {
        let transform = clip_scroll_tree
            .get_world_transform(prim_spatial_node_index);
        let matrix = transform.clone().into_transform().cast();

        // Apply the local clip rect here, before splitting. This is
        // because the local clip rect can't be applied in the vertex
        // shader for split composites, since we are drawing polygons
        // rather that rectangles. The interpolation still works correctly
        // since we determine the UVs by doing a bilerp with a factor
        // from the original local rect.
        let local_rect = match original_local_rect
            .intersection(combined_local_clip_rect)
        {
            Some(rect) => rect.cast(),
            None => return false,
        };
        let world_rect = world_rect.cast();

        match transform {
            CoordinateSpaceMapping::Local => {
                let polygon = Polygon::from_rect(
                    local_rect * TypedScale::new(1.0),
                    plane_split_anchor,
                );
                splitter.add(polygon);
            }
            CoordinateSpaceMapping::ScaleOffset(scale_offset) if scale_offset.scale == Vector2D::new(1.0, 1.0) => {
                let inv_matrix = scale_offset.inverse().to_transform().cast();
                let polygon = Polygon::from_transformed_rect_with_inverse(
                    local_rect,
                    &matrix,
                    &inv_matrix,
                    plane_split_anchor,
                ).unwrap();
                splitter.add(polygon);
            }
            CoordinateSpaceMapping::ScaleOffset(_) |
            CoordinateSpaceMapping::Transform(_) => {
                let mut clipper = Clipper::new();
                let results = clipper.clip_transformed(
                    Polygon::from_rect(
                        local_rect,
                        plane_split_anchor,
                    ),
                    &matrix,
                    Some(world_rect),
                );
                if let Ok(results) = results {
                    for poly in results {
                        splitter.add(poly);
                    }
                }
            }
        }

        true
    }

    pub fn resolve_split_planes(
        &mut self,
        splitter: &mut PlaneSplitter,
        gpu_cache: &mut GpuCache,
        clip_scroll_tree: &ClipScrollTree,
    ) {
        let ordered = match self.context_3d {
            Picture3DContext::In { root_data: Some(ref mut list), .. } => list,
            _ => panic!("Expected to find 3D context root"),
        };
        ordered.clear();

        // Process the accumulated split planes and order them for rendering.
        // Z axis is directed at the screen, `sort` is ascending, and we need back-to-front order.
        for poly in splitter.sort(vec3(0.0, 0.0, 1.0)) {
            let spatial_node_index = self.prim_list.prim_instances[poly.anchor].spatial_node_index;
            let transform = match clip_scroll_tree
                .get_world_transform(spatial_node_index)
                .inverse()
            {
                Some(transform) => transform.into_transform(),
                // logging this would be a bit too verbose
                None => continue,
            };

            let local_points = [
                transform.transform_point3d(&poly.points[0].cast()).unwrap(),
                transform.transform_point3d(&poly.points[1].cast()).unwrap(),
                transform.transform_point3d(&poly.points[2].cast()).unwrap(),
                transform.transform_point3d(&poly.points[3].cast()).unwrap(),
            ];
            let gpu_blocks = [
                [local_points[0].x, local_points[0].y, local_points[1].x, local_points[1].y].into(),
                [local_points[2].x, local_points[2].y, local_points[3].x, local_points[3].y].into(),
            ];
            let gpu_handle = gpu_cache.push_per_frame_blocks(&gpu_blocks);
            let gpu_address = gpu_cache.get_address(&gpu_handle);

            ordered.push(OrderedPictureChild {
                anchor: poly.anchor,
                spatial_node_index,
                gpu_address,
            });
        }
    }

    /// Called during initial picture traversal, before we know the
    /// bounding rect of children. It is possible to determine the
    /// surface / raster config now though.
    fn pre_update(
        &mut self,
        state: &mut PictureUpdateState,
        frame_context: &FrameBuildingContext,
    ) -> Option<PrimitiveList> {
        // Reset raster config in case we early out below.
        self.raster_config = None;

        // Resolve animation properties, and early out if the filter
        // properties make this picture invisible.
        if !self.resolve_scene_properties(frame_context.scene_properties) {
            return None;
        }

        // For out-of-preserve-3d pictures, the backface visibility is determined by
        // the local transform only.
        // Note: we aren't taking the transform relativce to the parent picture,
        // since picture tree can be more dense than the corresponding spatial tree.
        if !self.is_backface_visible {
            if let Picture3DContext::Out = self.context_3d {
                match frame_context.clip_scroll_tree.get_local_visible_face(self.spatial_node_index) {
                    VisibleFace::Front => {}
                    VisibleFace::Back => return None,
                }
            }
        }

        // Push information about this pic on stack for children to read.
        state.push_picture(PictureInfo {
            _spatial_node_index: self.spatial_node_index,
        });

        // See if this picture actually needs a surface for compositing.
        let actual_composite_mode = match self.requested_composite_mode {
            Some(PictureCompositeMode::Filter(ref filter)) if filter.is_noop() => None,
            Some(PictureCompositeMode::TileCache { .. }) => {
                // Disable tile cache if the scroll root has a perspective transform, since
                // this breaks many assumptions (it's a very rare edge case anyway, and
                // is probably (?) going to be moving / animated in this case).
                let spatial_node = &frame_context
                    .clip_scroll_tree
                    .spatial_nodes[self.spatial_node_index.0 as usize];
                if spatial_node.coordinate_system_id == CoordinateSystemId::root() {
                    Some(PictureCompositeMode::TileCache { })
                } else {
                    None
                }
            },
            ref mode => mode.clone(),
        };

        if let Some(composite_mode) = actual_composite_mode {
            // Retrieve the positioning node information for the parent surface.
            let parent_raster_node_index = state.current_surface().raster_spatial_node_index;
            let surface_spatial_node_index = self.spatial_node_index;

            // This inflation factor is to be applied to all primitives within the surface.
            let inflation_factor = match composite_mode {
                PictureCompositeMode::Filter(Filter::Blur(blur_radius)) => {
                    // Only inflate if the caller hasn't already inflated
                    // the bounding rects for this filter.
                    if self.options.inflate_if_required {
                        // The amount of extra space needed for primitives inside
                        // this picture to ensure the visibility check is correct.
                        BLUR_SAMPLE_SCALE * blur_radius
                    } else {
                        0.0
                    }
                }
                _ => {
                    0.0
                }
            };

            // Check if there is perspective, and thus whether a new
            // rasterization root should be established.
            let establishes_raster_root = frame_context.clip_scroll_tree
                .get_relative_transform(surface_spatial_node_index, parent_raster_node_index)
                .is_perspective();

            let surface = SurfaceInfo::new(
                surface_spatial_node_index,
                if establishes_raster_root {
                    surface_spatial_node_index
                } else {
                    parent_raster_node_index
                },
                inflation_factor,
                frame_context.global_screen_world_rect,
                &frame_context.clip_scroll_tree,
                frame_context.global_device_pixel_scale,
            );

            self.raster_config = Some(RasterConfig {
                composite_mode,
                establishes_raster_root,
                surface_index: state.push_surface(surface),
            });
        }

        Some(mem::replace(&mut self.prim_list, PrimitiveList::empty()))
    }

    /// Called after updating child pictures during the initial
    /// picture traversal.
    fn post_update(
        &mut self,
        prim_list: PrimitiveList,
        state: &mut PictureUpdateState,
        frame_context: &FrameBuildingContext,
    ) {
        // Restore the pictures list used during recursion.
        self.prim_list = prim_list;

        // Pop the state information about this picture.
        state.pop_picture();

        for cluster in &mut self.prim_list.clusters {
            // Skip the cluster if backface culled.
            if !cluster.is_backface_visible {
                // For in-preserve-3d primitives and pictures, the backface visibility is
                // evaluated relative to the containing block.
                if let Picture3DContext::In { ancestor_index, .. } = self.context_3d {
                    match frame_context.clip_scroll_tree
                        .get_relative_transform(cluster.spatial_node_index, ancestor_index)
                        .visible_face()
                    {
                        VisibleFace::Back => continue,
                        VisibleFace::Front => (),
                    }
                }
            }

            // No point including this cluster if it can't be transformed
            let spatial_node = &frame_context
                .clip_scroll_tree
                .spatial_nodes[cluster.spatial_node_index.0 as usize];
            if !spatial_node.invertible {
                continue;
            }

            // Map the cluster bounding rect into the space of the surface, and
            // include it in the surface bounding rect.
            let surface = state.current_surface_mut();
            surface.map_local_to_surface.set_target_spatial_node(
                cluster.spatial_node_index,
                frame_context.clip_scroll_tree,
            );

            // Mark the cluster visible, since it passed the invertible and
            // backface checks. In future, this will include spatial clustering
            // which will allow the frame building code to skip most of the
            // current per-primitive culling code.
            cluster.is_visible = true;
            if let Some(cluster_rect) = surface.map_local_to_surface.map(&cluster.bounding_rect) {
                surface.rect = surface.rect.union(&cluster_rect);
            }
        }

        // If this picture establishes a surface, then map the surface bounding
        // rect into the parent surface coordinate space, and propagate that up
        // to the parent.
        if let Some(ref mut raster_config) = self.raster_config {
            let mut surface_rect = {
                let surface = state.current_surface_mut();
                // Inflate the local bounding rect if required by the filter effect.
                // This inflaction factor is to be applied to the surface itsefl.
                // TODO: in prepare_for_render we round before multiplying with the
                // blur sample scale. Should we do this here as well?
                let inflation_size = match raster_config.composite_mode {
                    PictureCompositeMode::Filter(Filter::Blur(_)) => surface.inflation_factor,
                    PictureCompositeMode::Filter(Filter::DropShadows(ref shadows)) => {
                        let mut max = 0.0;
                        for shadow in shadows {
                            max = f32::max(max, shadow.blur_radius * BLUR_SAMPLE_SCALE);
                        }
                        max.ceil()
                    }
                    _ => 0.0,
                };
                surface.rect = surface.rect.inflate(inflation_size, inflation_size);
                surface.rect * TypedScale::new(1.0)
            };

            // Pop this surface from the stack
            let surface_index = state.pop_surface();
            debug_assert_eq!(surface_index, raster_config.surface_index);

            // Snapping may change the local rect slightly, and as such should just be
            // considered an estimated size for determining if we need raster roots and
            // preparing the tile cache.
            self.unsnapped_local_rect = surface_rect;

            // Check if any of the surfaces can't be rasterized in local space but want to.
            if raster_config.establishes_raster_root {
                if surface_rect.size.width > MAX_SURFACE_SIZE ||
                    surface_rect.size.height > MAX_SURFACE_SIZE
                {
                    raster_config.establishes_raster_root = false;
                    state.are_raster_roots_assigned = false;
                }
            }

            // Drop shadows draw both a content and shadow rect, so need to expand the local
            // rect of any surfaces to be composited in parent surfaces correctly.
            match raster_config.composite_mode {
                PictureCompositeMode::Filter(Filter::DropShadows(ref shadows)) => {
                    for shadow in shadows {
                        let content_rect = surface_rect;
                        let shadow_rect = surface_rect.translate(&shadow.offset);
                        surface_rect = content_rect.union(&shadow_rect);
                    }
                }
                _ => {}
            }

            // Propagate up to parent surface, now that we know this surface's static rect
            let parent_surface = state.current_surface_mut();
            parent_surface.map_local_to_surface.set_target_spatial_node(
                self.spatial_node_index,
                frame_context.clip_scroll_tree,
            );
            if let Some(parent_surface_rect) = parent_surface
                .map_local_to_surface
                .map(&surface_rect)
            {
                parent_surface.rect = parent_surface.rect.union(&parent_surface_rect);
            }
        }
    }

    pub fn prepare_for_render(
        &mut self,
        frame_context: &FrameBuildingContext,
        frame_state: &mut FrameBuildingState,
        data_stores: &mut DataStores,
    ) -> bool {
        let mut pic_state_for_children = self.take_state();

        if let Some(ref mut splitter) = pic_state_for_children.plane_splitter {
            self.resolve_split_planes(
                splitter,
                &mut frame_state.gpu_cache,
                &frame_context.clip_scroll_tree,
            );
        }

        let raster_config = match self.raster_config {
            Some(ref mut raster_config) => raster_config,
            None => {
                return true
            }
        };

        // TODO(gw): Almost all of the Picture types below use extra_gpu_cache_data
        //           to store the same type of data. The exception is the filter
        //           with a ColorMatrix, which stores the color matrix here. It's
        //           probably worth tidying this code up to be a bit more consistent.
        //           Perhaps store the color matrix after the common data, even though
        //           it's not used by that shader.

        match raster_config.composite_mode {
            PictureCompositeMode::TileCache { .. } => {}
            PictureCompositeMode::Filter(Filter::Blur(..)) => {}
            PictureCompositeMode::Filter(Filter::DropShadows(ref shadows)) => {
                self.extra_gpu_data_handles.resize(shadows.len(), GpuCacheHandle::new());
                for (shadow, extra_handle) in shadows.iter().zip(self.extra_gpu_data_handles.iter_mut()) {
                    if let Some(mut request) = frame_state.gpu_cache.request(extra_handle) {
                        // Basic brush primitive header is (see end of prepare_prim_for_render_inner in prim_store.rs)
                        //  [brush specific data]
                        //  [segment_rect, segment data]
                        let shadow_rect = self.snapped_local_rect.translate(&shadow.offset);

                        // ImageBrush colors
                        request.push(shadow.color.premultiplied());
                        request.push(PremultipliedColorF::WHITE);
                        request.push([
                            self.snapped_local_rect.size.width,
                            self.snapped_local_rect.size.height,
                            0.0,
                            0.0,
                        ]);

                        // segment rect / extra data
                        request.push(shadow_rect);
                        request.push([0.0, 0.0, 0.0, 0.0]);
                    }
                }
            }
            PictureCompositeMode::MixBlend(..) if !frame_context.fb_config.gpu_supports_advanced_blend => {}
            PictureCompositeMode::Filter(ref filter) => {
                match *filter {
                    Filter::ColorMatrix(ref m) => {
                        if self.extra_gpu_data_handles.is_empty() {
                            self.extra_gpu_data_handles.push(GpuCacheHandle::new());
                        }
                        if let Some(mut request) = frame_state.gpu_cache.request(&mut self.extra_gpu_data_handles[0]) {
                            for i in 0..5 {
                                request.push([m[i*4], m[i*4+1], m[i*4+2], m[i*4+3]]);
                            }
                        }
                    }
                    Filter::Flood(ref color) => {
                        if self.extra_gpu_data_handles.is_empty() {
                            self.extra_gpu_data_handles.push(GpuCacheHandle::new());
                        }
                        if let Some(mut request) = frame_state.gpu_cache.request(&mut self.extra_gpu_data_handles[0]) {
                            request.push(color.to_array());
                        }
                    }
                    _ => {}
                }
            }
            PictureCompositeMode::ComponentTransferFilter(handle) => {
                let filter_data = &mut data_stores.filter_data[handle];
                filter_data.update(frame_state);
            }
            PictureCompositeMode::MixBlend(..) |
            PictureCompositeMode::Blit(_) => {}
        }

        true
    }
}

// Calculate a single homogeneous screen-space UV for a picture.
fn calculate_screen_uv(
    local_pos: &PicturePoint,
    transform: &PictureToRasterTransform,
    rendered_rect: &DeviceRect,
    device_pixel_scale: DevicePixelScale,
    supports_snapping: bool,
) -> DeviceHomogeneousVector {
    let raster_pos = transform.transform_point2d_homogeneous(local_pos);

    let mut device_vec = DeviceHomogeneousVector::new(
        raster_pos.x * device_pixel_scale.0,
        raster_pos.y * device_pixel_scale.0,
        0.0,
        raster_pos.w,
    );

    // Apply snapping for axis-aligned scroll nodes, as per prim_shared.glsl.
    if transform.transform_kind() == TransformedRectKind::AxisAligned && supports_snapping {
        device_vec = DeviceHomogeneousVector::new(
            (device_vec.x / device_vec.w + 0.5).floor(),
            (device_vec.y / device_vec.w + 0.5).floor(),
            0.0,
            1.0,
        );
    }

    DeviceHomogeneousVector::new(
        (device_vec.x - rendered_rect.origin.x * device_vec.w) / rendered_rect.size.width,
        (device_vec.y - rendered_rect.origin.y * device_vec.w) / rendered_rect.size.height,
        0.0,
        device_vec.w,
    )
}

// Calculate a UV rect within an image based on the screen space
// vertex positions of a picture.
fn calculate_uv_rect_kind(
    pic_rect: &PictureRect,
    transform: &PictureToRasterTransform,
    rendered_rect: &DeviceIntRect,
    device_pixel_scale: DevicePixelScale,
    supports_snapping: bool,
) -> UvRectKind {
    let rendered_rect = rendered_rect.to_f32();

    let top_left = calculate_screen_uv(
        &pic_rect.origin,
        transform,
        &rendered_rect,
        device_pixel_scale,
        supports_snapping,
    );

    let top_right = calculate_screen_uv(
        &pic_rect.top_right(),
        transform,
        &rendered_rect,
        device_pixel_scale,
        supports_snapping,
    );

    let bottom_left = calculate_screen_uv(
        &pic_rect.bottom_left(),
        transform,
        &rendered_rect,
        device_pixel_scale,
        supports_snapping,
    );

    let bottom_right = calculate_screen_uv(
        &pic_rect.bottom_right(),
        transform,
        &rendered_rect,
        device_pixel_scale,
        supports_snapping,
    );

    UvRectKind::Quad {
        top_left,
        top_right,
        bottom_left,
        bottom_right,
    }
}

fn create_raster_mappers(
    surface_spatial_node_index: SpatialNodeIndex,
    raster_spatial_node_index: SpatialNodeIndex,
    world_rect: WorldRect,
    clip_scroll_tree: &ClipScrollTree,
) -> (SpaceMapper<RasterPixel, WorldPixel>, SpaceMapper<PicturePixel, RasterPixel>) {
    let map_raster_to_world = SpaceMapper::new_with_target(
        ROOT_SPATIAL_NODE_INDEX,
        raster_spatial_node_index,
        world_rect,
        clip_scroll_tree,
    );

    let raster_bounds = map_raster_to_world.unmap(&world_rect)
                                           .unwrap_or_else(RasterRect::max_rect);

    let map_pic_to_raster = SpaceMapper::new_with_target(
        raster_spatial_node_index,
        surface_spatial_node_index,
        raster_bounds,
        clip_scroll_tree,
    );

    (map_raster_to_world, map_pic_to_raster)
}
>>>>>>> upstream-releases
