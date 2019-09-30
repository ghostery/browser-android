/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use api::{AsyncBlobImageRasterizer, BlobImageRequest, BlobImageParams, BlobImageResult};
use api::{DocumentId, PipelineId, ApiMsg, FrameMsg, ResourceUpdate, ExternalEvent, Epoch};
use api::{BuiltDisplayList, ColorF, NotificationRequest, Checkpoint, IdNamespace};
use api::{ClipIntern, FilterDataIntern, MemoryReport, PrimitiveKeyKind};
use api::channel::MsgSender;
use api::units::LayoutSize;
#[cfg(feature = "capture")]
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
use capture::CaptureConfig;
use frame_builder::{FrameBuilderConfig, FrameBuilder};
use clip::{ClipDataInterner, ClipDataUpdateList};
use clip_scroll_tree::ClipScrollTree;
use display_list_flattener::DisplayListFlattener;
use intern::{Internable, Interner};
use internal_types::{FastHashMap, FastHashSet};
use prim_store::{PrimitiveDataInterner, PrimitiveDataUpdateList, PrimitiveKeyKind};
use prim_store::PrimitiveStoreStats;
use prim_store::gradient::{
    LinearGradient, LinearGradientDataInterner, LinearGradientDataUpdateList,
    RadialGradient, RadialGradientDataInterner, RadialGradientDataUpdateList
};
use prim_store::text_run::{TextRunDataInterner, TextRun, TextRunDataUpdateList};
use resource_cache::{BlobImageRasterizerEpoch, FontInstanceMap};
use render_backend::DocumentView;
use renderer::{PipelineInfo, SceneBuilderHooks};
use scene::Scene;
||||||| merged common ancestors
use capture::CaptureConfig;
use frame_builder::{FrameBuilderConfig, FrameBuilder};
use clip::{ClipDataInterner, ClipDataUpdateList};
use clip_scroll_tree::ClipScrollTree;
use display_list_flattener::DisplayListFlattener;
use internal_types::{FastHashMap, FastHashSet};
use picture::PictureIdGenerator;
use prim_store::{PrimitiveDataInterner, PrimitiveDataUpdateList};
use resource_cache::FontInstanceMap;
use render_backend::DocumentView;
use renderer::{PipelineInfo, SceneBuilderHooks};
use scene::Scene;
=======
use crate::capture::CaptureConfig;
use crate::frame_builder::{FrameBuilderConfig, FrameBuilder};
use crate::clip_scroll_tree::ClipScrollTree;
use crate::display_list_flattener::DisplayListFlattener;
use crate::hit_test::HitTestingSceneStats;
use crate::intern::{Internable, Interner, UpdateList};
use crate::internal_types::{FastHashMap, FastHashSet};
use malloc_size_of::{MallocSizeOf, MallocSizeOfOps};
use crate::prim_store::PrimitiveStoreStats;
use crate::prim_store::borders::{ImageBorder, NormalBorderPrim};
use crate::prim_store::gradient::{LinearGradient, RadialGradient};
use crate::prim_store::image::{Image, YuvImage};
use crate::prim_store::line_dec::LineDecoration;
use crate::prim_store::picture::Picture;
use crate::prim_store::text_run::TextRun;
use crate::resource_cache::{AsyncBlobImageInfo, FontInstanceMap};
use crate::render_backend::DocumentView;
use crate::renderer::{PipelineInfo, SceneBuilderHooks};
use crate::scene::Scene;
use std::iter;
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
use std::sync::mpsc::{channel, Receiver, Sender};
use std::mem::replace;
use time::precise_time_ns;
use crate::util::drain_filter;
use std::thread;
use std::time::Duration;

<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
pub struct DocumentResourceUpdates {
    pub clip_updates: ClipDataUpdateList,
    pub prim_updates: PrimitiveDataUpdateList,
    pub linear_grad_updates: LinearGradientDataUpdateList,
    pub radial_grad_updates: RadialGradientDataUpdateList,
    pub text_run_updates: TextRunDataUpdateList,
}
||||||| merged common ancestors
pub struct DocumentResourceUpdates {
    pub clip_updates: ClipDataUpdateList,
    pub prim_updates: PrimitiveDataUpdateList,
}
=======
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs

/// Represents the work associated to a transaction before scene building.
pub struct Transaction {
    pub document_id: DocumentId,
    pub display_list_updates: Vec<DisplayListUpdate>,
    pub removed_pipelines: Vec<(PipelineId, DocumentId)>,
    pub epoch_updates: Vec<(PipelineId, Epoch)>,
    pub request_scene_build: Option<SceneRequest>,
    pub blob_requests: Vec<BlobImageParams>,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    pub blob_rasterizer: Option<(Box<AsyncBlobImageRasterizer>, BlobImageRasterizerEpoch)>,
||||||| merged common ancestors
    pub blob_rasterizer: Option<Box<AsyncBlobImageRasterizer>>,
=======
    pub blob_rasterizer: Option<(Box<dyn AsyncBlobImageRasterizer>, AsyncBlobImageInfo)>,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    pub rasterized_blobs: Vec<(BlobImageRequest, BlobImageResult)>,
    pub resource_updates: Vec<ResourceUpdate>,
    pub frame_ops: Vec<FrameMsg>,
    pub notifications: Vec<NotificationRequest>,
    pub set_root_pipeline: Option<PipelineId>,
    pub render_frame: bool,
    pub invalidate_rendered_frame: bool,
}

impl Transaction {
    pub fn can_skip_scene_builder(&self) -> bool {
        self.request_scene_build.is_none() &&
            self.display_list_updates.is_empty() &&
            self.epoch_updates.is_empty() &&
            self.removed_pipelines.is_empty() &&
            self.blob_requests.is_empty() &&
            self.set_root_pipeline.is_none()
    }

    pub fn should_build_scene(&self) -> bool {
        !self.display_list_updates.is_empty() ||
            self.set_root_pipeline.is_some()
    }
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs

    fn rasterize_blobs(&mut self, is_low_priority: bool) {
        if let Some((ref mut rasterizer, _)) = self.blob_rasterizer {
            let rasterized_blobs = rasterizer.rasterize(&self.blob_requests, is_low_priority);
            self.rasterized_blobs.extend(rasterized_blobs);
        }
    }
||||||| merged common ancestors
=======

    fn rasterize_blobs(&mut self, is_low_priority: bool) {
        if let Some((ref mut rasterizer, _)) = self.blob_rasterizer {
            let mut rasterized_blobs = rasterizer.rasterize(&self.blob_requests, is_low_priority);
            // try using the existing allocation if our current list is empty
            if self.rasterized_blobs.is_empty() {
                self.rasterized_blobs = rasterized_blobs;
            } else {
                self.rasterized_blobs.append(&mut rasterized_blobs);
            }
        }
    }
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
}

/// Represent the remaining work associated to a transaction after the scene building
/// phase as well as the result of scene building itself if applicable.
pub struct BuiltTransaction {
    pub document_id: DocumentId,
    pub built_scene: Option<BuiltScene>,
    pub resource_updates: Vec<ResourceUpdate>,
    pub rasterized_blobs: Vec<(BlobImageRequest, BlobImageResult)>,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    pub blob_rasterizer: Option<(Box<AsyncBlobImageRasterizer>, BlobImageRasterizerEpoch)>,
||||||| merged common ancestors
    pub blob_rasterizer: Option<Box<AsyncBlobImageRasterizer>>,
=======
    pub blob_rasterizer: Option<(Box<dyn AsyncBlobImageRasterizer>, AsyncBlobImageInfo)>,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    pub frame_ops: Vec<FrameMsg>,
    pub removed_pipelines: Vec<(PipelineId, DocumentId)>,
    pub notifications: Vec<NotificationRequest>,
    pub interner_updates: Option<InternerUpdates>,
    pub scene_build_start_time: u64,
    pub scene_build_end_time: u64,
    pub render_frame: bool,
    pub invalidate_rendered_frame: bool,
}

pub struct DisplayListUpdate {
    pub pipeline_id: PipelineId,
    pub epoch: Epoch,
    pub built_display_list: BuiltDisplayList,
    pub background: Option<ColorF>,
    pub viewport_size: LayoutSize,
    pub content_size: LayoutSize,
}

/// Contains the render backend data needed to build a scene.
pub struct SceneRequest {
    pub view: DocumentView,
    pub font_instances: FontInstanceMap,
    pub output_pipelines: FastHashSet<PipelineId>,
}

#[cfg(feature = "replay")]
pub struct LoadScene {
    pub document_id: DocumentId,
    pub scene: Scene,
    pub output_pipelines: FastHashSet<PipelineId>,
    pub font_instances: FontInstanceMap,
    pub view: DocumentView,
    pub config: FrameBuilderConfig,
    pub build_frame: bool,
    pub interners: Interners,
}

pub struct BuiltScene {
    pub scene: Scene,
    pub frame_builder: FrameBuilder,
    pub clip_scroll_tree: ClipScrollTree,
}

// Message from render backend to scene builder.
pub enum SceneBuilderRequest {
    Transactions(Vec<Box<Transaction>>),
    ExternalEvent(ExternalEvent),
    DeleteDocument(DocumentId),
    WakeUp,
    Flush(MsgSender<()>),
    ClearNamespace(IdNamespace),
    SetFrameBuilderConfig(FrameBuilderConfig),
    SimulateLongSceneBuild(u32),
    SimulateLongLowPrioritySceneBuild(u32),
    Stop,
    ReportMemory(MemoryReport, MsgSender<MemoryReport>),
    #[cfg(feature = "capture")]
    SaveScene(CaptureConfig),
    #[cfg(feature = "replay")]
    LoadScenes(Vec<LoadScene>),
}

// Message from scene builder to render backend.
pub enum SceneBuilderResult {
    Transactions(Vec<Box<BuiltTransaction>>, Option<Sender<SceneSwapResult>>),
    ExternalEvent(ExternalEvent),
    FlushComplete(MsgSender<()>),
    ClearNamespace(IdNamespace),
    Stopped,
}

// Message from render backend to scene builder to indicate the
// scene swap was completed. We need a separate channel for this
// so that they don't get mixed with SceneBuilderRequest messages.
pub enum SceneSwapResult {
    Complete(Sender<()>),
    Aborted,
}

<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
// This struct contains all items that can be shared between
// display lists. We want to intern and share the same clips,
// primitives and other things between display lists so that:
// - GPU cache handles remain valid, reducing GPU cache updates.
// - Comparison of primitives and pictures between two
//   display lists is (a) fast (b) done during scene building.
#[cfg_attr(feature = "capture", derive(Serialize))]
#[cfg_attr(feature = "replay", derive(Deserialize))]
#[derive(Default)]
pub struct DocumentResources {
    pub clip_interner: ClipDataInterner,
    pub prim_interner: PrimitiveDataInterner,
    pub linear_grad_interner: LinearGradientDataInterner,
    pub radial_grad_interner: RadialGradientDataInterner,
    pub text_run_interner: TextRunDataInterner,
||||||| merged common ancestors
// This struct contains all items that can be shared between
// display lists. We want to intern and share the same clips,
// primitives and other things between display lists so that:
// - GPU cache handles remain valid, reducing GPU cache updates.
// - Comparison of primitives and pictures between two
//   display lists is (a) fast (b) done during scene building.
#[cfg_attr(feature = "capture", derive(Serialize))]
#[cfg_attr(feature = "replay", derive(Deserialize))]
pub struct DocumentResources {
    pub clip_interner: ClipDataInterner,
    pub prim_interner: PrimitiveDataInterner,
=======
macro_rules! declare_interners {
    ( $( $name:ident : $ty:ident, )+ ) => {
        /// This struct contains all items that can be shared between
        /// display lists. We want to intern and share the same clips,
        /// primitives and other things between display lists so that:
        /// - GPU cache handles remain valid, reducing GPU cache updates.
        /// - Comparison of primitives and pictures between two
        ///   display lists is (a) fast (b) done during scene building.
        #[cfg_attr(feature = "capture", derive(Serialize))]
        #[cfg_attr(feature = "replay", derive(Deserialize))]
        #[derive(Default)]
        pub struct Interners {
            $(
                pub $name: Interner<$ty>,
            )+
        }

        $(
            impl AsMut<Interner<$ty>> for Interners {
                fn as_mut(&mut self) -> &mut Interner<$ty> {
                    &mut self.$name
                }
            }
        )+

        pub struct InternerUpdates {
            $(
                pub $name: UpdateList<<$ty as Internable>::Key>,
            )+
        }

        impl Interners {
            /// Reports CPU heap memory used by the interners.
            fn report_memory(
                &self,
                ops: &mut MallocSizeOfOps,
                r: &mut MemoryReport,
            ) {
                $(
                    r.interning.interners.$name += self.$name.size_of(ops);
                )+
            }

            fn end_frame_and_get_pending_updates(&mut self) -> InternerUpdates {
                InternerUpdates {
                    $(
                        $name: self.$name.end_frame_and_get_pending_updates(),
                    )+
                }
            }
        }
    }
}

enumerate_interners!(declare_interners);

/// Stores the allocation sizes of various arrays in the frame
/// builder. This is retrieved from the current frame builder
/// and used to reserve an approximately correct capacity of
/// the arrays for the next scene that is getting built.
pub struct DocumentStats {
    pub prim_store_stats: PrimitiveStoreStats,
    pub hit_test_stats: HitTestingSceneStats,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
}

<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
// Access to `DocumentResources` interners by `Internable`
pub trait InternerMut<I: Internable>
{
    fn interner_mut(&mut self) -> &mut Interner<I::Source, I::InternData, I::Marker>;
}

impl InternerMut<PrimitiveKeyKind> for DocumentResources {
    fn interner_mut(&mut self) -> &mut PrimitiveDataInterner {
        &mut self.prim_interner
    }
}

impl InternerMut<LinearGradient> for DocumentResources {
    fn interner_mut(&mut self) -> &mut LinearGradientDataInterner {
        &mut self.linear_grad_interner
    }
}

impl InternerMut<RadialGradient> for DocumentResources {
    fn interner_mut(&mut self) -> &mut RadialGradientDataInterner {
        &mut self.radial_grad_interner
    }
}

impl InternerMut<TextRun> for DocumentResources {
    fn interner_mut(&mut self) -> &mut TextRunDataInterner {
        &mut self.text_run_interner
||||||| merged common ancestors
impl DocumentResources {
    fn new() -> Self {
        DocumentResources {
            clip_interner: ClipDataInterner::new(),
            prim_interner: PrimitiveDataInterner::new(),
        }
=======
impl DocumentStats {
    pub fn empty() -> DocumentStats {
        DocumentStats {
            prim_store_stats: PrimitiveStoreStats::empty(),
            hit_test_stats: HitTestingSceneStats::empty(),
        }
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    }
}


// A document in the scene builder contains the current scene,
// as well as a persistent clip interner. This allows clips
// to be de-duplicated, and persisted in the GPU cache between
// display lists.
struct Document {
    scene: Scene,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    resources: DocumentResources,
    prim_store_stats: PrimitiveStoreStats,
||||||| merged common ancestors
    resources: DocumentResources,
=======
    interners: Interners,
    doc_stats: DocumentStats,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
}

impl Document {
    fn new(scene: Scene) -> Self {
        Document {
            scene,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
            resources: DocumentResources::default(),
            prim_store_stats: PrimitiveStoreStats::empty(),
||||||| merged common ancestors
            resources: DocumentResources::new(),
=======
            interners: Interners::default(),
            doc_stats: DocumentStats::empty(),
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
        }
    }
}

pub struct SceneBuilder {
    documents: FastHashMap<DocumentId, Document>,
    rx: Receiver<SceneBuilderRequest>,
    tx: Sender<SceneBuilderResult>,
    api_tx: MsgSender<ApiMsg>,
    config: FrameBuilderConfig,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    hooks: Option<Box<SceneBuilderHooks + Send>>,
||||||| merged common ancestors
    hooks: Option<Box<SceneBuilderHooks + Send>>,
    picture_id_generator: PictureIdGenerator,
=======
    hooks: Option<Box<dyn SceneBuilderHooks + Send>>,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
    simulate_slow_ms: u32,
    size_of_ops: Option<MallocSizeOfOps>,
}

impl SceneBuilder {
    pub fn new(
        config: FrameBuilderConfig,
        api_tx: MsgSender<ApiMsg>,
        hooks: Option<Box<dyn SceneBuilderHooks + Send>>,
        size_of_ops: Option<MallocSizeOfOps>,
    ) -> (Self, Sender<SceneBuilderRequest>, Receiver<SceneBuilderResult>) {
        let (in_tx, in_rx) = channel();
        let (out_tx, out_rx) = channel();
        (
            SceneBuilder {
                documents: FastHashMap::default(),
                rx: in_rx,
                tx: out_tx,
                api_tx,
                config,
                hooks,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
||||||| merged common ancestors
                picture_id_generator: PictureIdGenerator::new(),
=======
                size_of_ops,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                simulate_slow_ms: 0,
            },
            in_tx,
            out_rx,
        )
    }

    /// Send a message to the render backend thread.
    ///
    /// We first put something in the result queue and then send a wake-up
    /// message to the api queue that the render backend is blocking on.
    pub fn send(&self, msg: SceneBuilderResult) {
        self.tx.send(msg).unwrap();
        let _ = self.api_tx.send(ApiMsg::WakeUp);
    }

    /// The scene builder thread's event loop.
    pub fn run(&mut self) {
        if let Some(ref hooks) = self.hooks {
            hooks.register();
        }

        loop {
            match self.rx.recv() {
                Ok(SceneBuilderRequest::WakeUp) => {}
                Ok(SceneBuilderRequest::Flush(tx)) => {
                    self.send(SceneBuilderResult::FlushComplete(tx));
                }
                Ok(SceneBuilderRequest::Transactions(mut txns)) => {
                    let built_txns : Vec<Box<BuiltTransaction>> = txns.iter_mut()
                        .map(|txn| self.process_transaction(txn))
                        .collect();
                    self.forward_built_transactions(built_txns);
                }
                Ok(SceneBuilderRequest::DeleteDocument(document_id)) => {
                    self.documents.remove(&document_id);
                }
                Ok(SceneBuilderRequest::SetFrameBuilderConfig(cfg)) => {
                    self.config = cfg;
                }
                Ok(SceneBuilderRequest::ClearNamespace(id)) => {
                    self.documents.retain(|doc_id, _doc| doc_id.namespace_id != id);
                    self.send(SceneBuilderResult::ClearNamespace(id));
                }
                #[cfg(feature = "replay")]
                Ok(SceneBuilderRequest::LoadScenes(msg)) => {
                    self.load_scenes(msg);
                }
                #[cfg(feature = "capture")]
                Ok(SceneBuilderRequest::SaveScene(config)) => {
                    self.save_scene(config);
                }
                Ok(SceneBuilderRequest::ExternalEvent(evt)) => {
                    self.send(SceneBuilderResult::ExternalEvent(evt));
                }
                Ok(SceneBuilderRequest::Stop) => {
                    self.tx.send(SceneBuilderResult::Stopped).unwrap();
                    // We don't need to send a WakeUp to api_tx because we only
                    // get the Stop when the RenderBackend loop is exiting.
                    break;
                }
                Ok(SceneBuilderRequest::ReportMemory(mut report, tx)) => {
                    report += self.report_memory();
                    tx.send(report).unwrap();
                }
                Ok(SceneBuilderRequest::SimulateLongSceneBuild(time_ms)) => {
                    self.simulate_slow_ms = time_ms
                }
                Ok(SceneBuilderRequest::SimulateLongLowPrioritySceneBuild(_)) => {}
                Err(_) => {
                    break;
                }
            }

            if let Some(ref hooks) = self.hooks {
                hooks.poke();
            }
        }

        if let Some(ref hooks) = self.hooks {
            hooks.deregister();
        }
    }

    #[cfg(feature = "capture")]
    fn save_scene(&mut self, config: CaptureConfig) {
        for (id, doc) in &self.documents {
            let interners_name = format!("interners-{}-{}", id.namespace_id.0, id.id);
            config.serialize(&doc.interners, interners_name);
        }
    }

    #[cfg(feature = "replay")]
    fn load_scenes(&mut self, scenes: Vec<LoadScene>) {
        for mut item in scenes {
            self.config = item.config;

            let scene_build_start_time = precise_time_ns();

            let mut built_scene = None;
            let mut interner_updates = None;

            if item.scene.has_root_pipeline() {
                let mut clip_scroll_tree = ClipScrollTree::new();
                let mut new_scene = Scene::new();

                let frame_builder = DisplayListFlattener::create_frame_builder(
                    &item.scene,
                    &mut clip_scroll_tree,
                    item.font_instances,
                    &item.view,
                    &item.output_pipelines,
                    &self.config,
                    &mut new_scene,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                    &mut item.doc_resources,
                    &PrimitiveStoreStats::empty(),
||||||| merged common ancestors
                    item.scene_id,
                    &mut self.picture_id_generator,
                    &mut item.doc_resources,
=======
                    &mut item.interners,
                    &DocumentStats::empty(),
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                );

<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                // TODO(djg): Can we do better than this?  Use a #[derive] to
                // write the code for us, or unify updates into one enum/list?
                let clip_updates = item
                    .doc_resources
                    .clip_interner
                    .end_frame_and_get_pending_updates();

                let prim_updates = item
                    .doc_resources
                    .prim_interner
                    .end_frame_and_get_pending_updates();

                let linear_grad_updates = item
                    .doc_resources
                    .linear_grad_interner
                    .end_frame_and_get_pending_updates();

                let radial_grad_updates = item
                    .doc_resources
                    .radial_grad_interner
                    .end_frame_and_get_pending_updates();

                let text_run_updates = item
                    .doc_resources
                    .text_run_interner
                    .end_frame_and_get_pending_updates();

                doc_resource_updates = Some(
                    DocumentResourceUpdates {
                        clip_updates,
                        prim_updates,
                        linear_grad_updates,
                        radial_grad_updates,
                        text_run_updates,
                    }
||||||| merged common ancestors
                let clip_updates = item
                    .doc_resources
                    .clip_interner
                    .end_frame_and_get_pending_updates();

                let prim_updates = item
                    .doc_resources
                    .prim_interner
                    .end_frame_and_get_pending_updates();

                doc_resource_updates = Some(
                    DocumentResourceUpdates {
                        clip_updates,
                        prim_updates,
                    }
=======
                interner_updates = Some(
                    item.interners.end_frame_and_get_pending_updates()
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                );

                built_scene = Some(BuiltScene {
                    scene: new_scene,
                    frame_builder,
                    clip_scroll_tree,
                });
            }

            self.documents.insert(
                item.document_id,
                Document {
                    scene: item.scene,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                    resources: item.doc_resources,
                    prim_store_stats: PrimitiveStoreStats::empty(),
||||||| merged common ancestors
                    resources: item.doc_resources,
=======
                    interners: item.interners,
                    doc_stats: DocumentStats::empty(),
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                },
            );

            let txns = vec![Box::new(BuiltTransaction {
                document_id: item.document_id,
                render_frame: item.build_frame,
                invalidate_rendered_frame: false,
                built_scene,
                resource_updates: Vec::new(),
                rasterized_blobs: Vec::new(),
                blob_rasterizer: None,
                frame_ops: Vec::new(),
                removed_pipelines: Vec::new(),
                notifications: Vec::new(),
                scene_build_start_time,
                scene_build_end_time: precise_time_ns(),
                interner_updates,
            })];

            self.forward_built_transactions(txns);
        }
    }

    /// Do the bulk of the work of the scene builder thread.
    fn process_transaction(&mut self, txn: &mut Transaction) -> Box<BuiltTransaction> {
        if let &Some(ref hooks) = &self.hooks {
            hooks.pre_scene_build();
        }

        let scene_build_start_time = precise_time_ns();

        let doc = self.documents
                      .entry(txn.document_id)
                      .or_insert_with(|| Document::new(Scene::new()));
        let scene = &mut doc.scene;

        for update in txn.display_list_updates.drain(..) {
            scene.set_display_list(
                update.pipeline_id,
                update.epoch,
                update.built_display_list,
                update.background,
                update.viewport_size,
                update.content_size,
            );
        }

        for &(pipeline_id, epoch) in &txn.epoch_updates {
            scene.update_epoch(pipeline_id, epoch);
        }

        if let Some(id) = txn.set_root_pipeline {
            scene.set_root_pipeline_id(id);
        }

        for &(pipeline_id, _) in &txn.removed_pipelines {
            scene.remove_pipeline(pipeline_id)
        }

        let mut built_scene = None;
        let mut interner_updates = None;
        if scene.has_root_pipeline() {
            if let Some(request) = txn.request_scene_build.take() {
                let mut clip_scroll_tree = ClipScrollTree::new();
                let mut new_scene = Scene::new();

                let frame_builder = DisplayListFlattener::create_frame_builder(
                    &scene,
                    &mut clip_scroll_tree,
                    request.font_instances,
                    &request.view,
                    &request.output_pipelines,
                    &self.config,
                    &mut new_scene,
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                    &mut doc.resources,
                    &doc.prim_store_stats,
||||||| merged common ancestors
                    request.scene_id,
                    &mut self.picture_id_generator,
                    &mut doc.resources,
=======
                    &mut doc.interners,
                    &doc.doc_stats,
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                );

<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                // Update the allocation stats for next scene
                doc.prim_store_stats = frame_builder.prim_store.get_stats();

||||||| merged common ancestors
=======
                // Update the allocation stats for next scene
                doc.doc_stats = frame_builder.get_stats();

>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                // Retrieve the list of updates from the clip interner.
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                let clip_updates = doc
                    .resources
                    .clip_interner
                    .end_frame_and_get_pending_updates();

                let prim_updates = doc
                    .resources
                    .prim_interner
                    .end_frame_and_get_pending_updates();

                let linear_grad_updates = doc
                    .resources
                    .linear_grad_interner
                    .end_frame_and_get_pending_updates();

                let radial_grad_updates = doc
                    .resources
                    .radial_grad_interner
                    .end_frame_and_get_pending_updates();

                let text_run_updates = doc
                    .resources
                    .text_run_interner
                    .end_frame_and_get_pending_updates();

                doc_resource_updates = Some(
                    DocumentResourceUpdates {
                        clip_updates,
                        prim_updates,
                        linear_grad_updates,
                        radial_grad_updates,
                        text_run_updates,
                    }
||||||| merged common ancestors
                let clip_updates = doc
                    .resources
                    .clip_interner
                    .end_frame_and_get_pending_updates();

                let prim_updates = doc
                    .resources
                    .prim_interner
                    .end_frame_and_get_pending_updates();

                doc_resource_updates = Some(
                    DocumentResourceUpdates {
                        clip_updates,
                        prim_updates,
                    }
=======
                interner_updates = Some(
                    doc.interners.end_frame_and_get_pending_updates()
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
                );

                built_scene = Some(BuiltScene {
                    scene: new_scene,
                    frame_builder,
                    clip_scroll_tree,
                });
            }
        }

        let is_low_priority = false;
        txn.rasterize_blobs(is_low_priority);

        drain_filter(
            &mut txn.notifications,
            |n| { n.when() == Checkpoint::SceneBuilt },
            |n| { n.notify(); },
        );

        if self.simulate_slow_ms > 0 {
            thread::sleep(Duration::from_millis(self.simulate_slow_ms as u64));
        }

        Box::new(BuiltTransaction {
            document_id: txn.document_id,
            render_frame: txn.render_frame,
            invalidate_rendered_frame: txn.invalidate_rendered_frame,
            built_scene,
            rasterized_blobs: replace(&mut txn.rasterized_blobs, Vec::new()),
            resource_updates: replace(&mut txn.resource_updates, Vec::new()),
            blob_rasterizer: replace(&mut txn.blob_rasterizer, None),
            frame_ops: replace(&mut txn.frame_ops, Vec::new()),
            removed_pipelines: replace(&mut txn.removed_pipelines, Vec::new()),
            notifications: replace(&mut txn.notifications, Vec::new()),
            interner_updates,
            scene_build_start_time,
            scene_build_end_time: precise_time_ns(),
        })
    }

    /// Send the results of process_transaction back to the render backend.
    fn forward_built_transactions(&mut self, txns: Vec<Box<BuiltTransaction>>) {
        let (pipeline_info, result_tx, result_rx) = match &self.hooks {
            &Some(ref hooks) => {
                if txns.iter().any(|txn| txn.built_scene.is_some()) {
                    let info = PipelineInfo {
                        epochs: txns.iter()
                            .filter(|txn| txn.built_scene.is_some())
                            .map(|txn| {
                                txn.built_scene.as_ref().unwrap()
                                    .scene.pipeline_epochs.iter()
                                    .zip(iter::repeat(txn.document_id))
                                    .map(|((&pipeline_id, &epoch), document_id)| ((pipeline_id, document_id), epoch))
                            }).flatten().collect(),
                        removed_pipelines: txns.iter()
                            .map(|txn| txn.removed_pipelines.clone())
                            .flatten().collect(),
                    };

                    let (tx, rx) = channel();
                    let txn = txns.iter().find(|txn| txn.built_scene.is_some()).unwrap();
                    hooks.pre_scene_swap(txn.scene_build_end_time - txn.scene_build_start_time);

                    (Some(info), Some(tx), Some(rx))
                } else {
                    (None, None, None)
                }
            }
            _ => (None, None, None)
        };

        let scene_swap_start_time = precise_time_ns();
        let document_ids = txns.iter().map(|txn| txn.document_id).collect();
        let have_resources_updates : Vec<DocumentId> = if pipeline_info.is_none() {
            txns.iter()
                .filter(|txn| !txn.resource_updates.is_empty() || txn.invalidate_rendered_frame)
                .map(|txn| txn.document_id.clone())
                .collect()
        } else {
            Vec::new()
        };

        self.tx.send(SceneBuilderResult::Transactions(txns, result_tx)).unwrap();

        let _ = self.api_tx.send(ApiMsg::WakeUp);

        if let Some(pipeline_info) = pipeline_info {
            // Block until the swap is done, then invoke the hook.
            let swap_result = result_rx.unwrap().recv();
            let scene_swap_time = precise_time_ns() - scene_swap_start_time;
            self.hooks.as_ref().unwrap().post_scene_swap(&document_ids,
                                                         pipeline_info, scene_swap_time);
            // Once the hook is done, allow the RB thread to resume
            match swap_result {
                Ok(SceneSwapResult::Complete(resume_tx)) => {
                    resume_tx.send(()).ok();
                },
                _ => (),
            };
        } else if !have_resources_updates.is_empty() {
            if let &Some(ref hooks) = &self.hooks {
                hooks.post_resource_update(&have_resources_updates);
            }
        } else {
            if let &Some(ref hooks) = &self.hooks {
                hooks.post_empty_scene_build();
            }
        } else {
            if let &Some(ref hooks) = &self.hooks {
                hooks.post_empty_scene_build();
            }
        }
    }

    /// Reports CPU heap memory used by the SceneBuilder.
    fn report_memory(&mut self) -> MemoryReport {
        let ops = self.size_of_ops.as_mut().unwrap();
        let mut report = MemoryReport::default();
        for doc in self.documents.values() {
            doc.interners.report_memory(ops, &mut report);
        }

        report
    }
}

/// A scene builder thread which executes expensive operations such as blob rasterization
/// with a lower priority than the normal scene builder thread.
///
/// After rasterizing blobs, the secene building request is forwarded to the normal scene
/// builder where the FrameBuilder is generated.
pub struct LowPrioritySceneBuilder {
    pub rx: Receiver<SceneBuilderRequest>,
    pub tx: Sender<SceneBuilderRequest>,
    pub simulate_slow_ms: u32,
}

impl LowPrioritySceneBuilder {
    pub fn run(&mut self) {
        loop {
            match self.rx.recv() {
                Ok(SceneBuilderRequest::Transactions(mut txns)) => {
                    let txns : Vec<Box<Transaction>> = txns.drain(..)
                        .map(|txn| self.process_transaction(txn))
                        .collect();
                    self.tx.send(SceneBuilderRequest::Transactions(txns)).unwrap();
                }
                Ok(SceneBuilderRequest::DeleteDocument(document_id)) => {
                    self.tx.send(SceneBuilderRequest::DeleteDocument(document_id)).unwrap();
                }
                Ok(SceneBuilderRequest::Stop) => {
                    self.tx.send(SceneBuilderRequest::Stop).unwrap();
                    break;
                }
                Ok(SceneBuilderRequest::SimulateLongLowPrioritySceneBuild(time_ms)) => {
                    self.simulate_slow_ms = time_ms;
                }
                Ok(other) => {
                    self.tx.send(other).unwrap();
                }
                Err(_) => {
                    break;
                }
            }
        }
    }

    fn process_transaction(&mut self, mut txn: Box<Transaction>) -> Box<Transaction> {
        let is_low_priority = true;
<<<<<<< HEAD:mozilla-release/gfx/wr/webrender/src/scene_builder.rs
        txn.rasterize_blobs(is_low_priority);
||||||| merged common ancestors
        let mut more_rasterized_blobs = txn.blob_rasterizer.as_mut().map_or(
            Vec::new(),
            |rasterizer| rasterizer.rasterize(&blob_requests, is_low_priority),
        );
        txn.rasterized_blobs.append(&mut more_rasterized_blobs);
=======
        txn.rasterize_blobs(is_low_priority);
        txn.blob_requests = Vec::new();
>>>>>>> upstream-releases:mozilla-release/gfx/wr/webrender/src/scene_builder.rs

        if self.simulate_slow_ms > 0 {
            thread::sleep(Duration::from_millis(self.simulate_slow_ms as u64));
        }

        txn
    }
}
