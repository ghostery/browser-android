// Copyright © 2017 Mozilla Foundation
//
// This program is made available under an ISC-style license.  See the
// accompanying file LICENSE for details

#[macro_use]
extern crate error_chain;

#[macro_use]
extern crate log;

extern crate audioipc;
extern crate bytes;
extern crate cubeb_core as cubeb;
extern crate futures;
extern crate lazycell;
extern crate libc;
extern crate slab;
extern crate tokio_core;
extern crate tokio_uds;
extern crate audio_thread_priority;

use audioipc::core;
<<<<<<< HEAD
use audioipc::fd_passing::framed_with_fds;
||||||| merged common ancestors
use audioipc::fd_passing::{framed_with_fds, FramedWithFds};
use audioipc::frame::{framed, Framed};
use audioipc::messages::{CallbackReq, CallbackResp, ClientMessage, Device, DeviceInfo,
                         ServerMessage, StreamCreate, StreamInitParams, StreamParams};
=======
use audioipc::platformhandle_passing::framed_with_platformhandles;
>>>>>>> upstream-releases
use audioipc::rpc;
<<<<<<< HEAD
use audioipc::PlatformHandleType;
||||||| merged common ancestors
use audioipc::shm::{SharedMemReader, SharedMemWriter};
use cubeb::ffi;
use futures::future::{self, FutureResult};
=======
use audioipc::{MessageStream, PlatformHandle, PlatformHandleType};
>>>>>>> upstream-releases
use futures::sync::oneshot;
use futures::Future;
use std::error::Error;
<<<<<<< HEAD
use std::os::raw::c_void;
use std::os::unix::io::IntoRawFd;
use std::os::unix::net;
use std::ptr;
use tokio_uds::UnixStream;
||||||| merged common ancestors
use std::ffi::{CStr, CString};
use std::mem::{size_of, ManuallyDrop};
use std::os::raw::{c_long, c_void};
use std::os::unix::net;
use std::os::unix::prelude::*;
use std::{panic, ptr, slice};
use tokio_core::reactor::Remote;
use tokio_uds::UnixStream;
=======
use std::os::raw::c_void;
use std::ptr;
use audio_thread_priority::promote_current_thread_to_real_time;

mod server;
>>>>>>> upstream-releases

<<<<<<< HEAD
mod server;

||||||| merged common ancestors
=======
#[allow(deprecated)]
>>>>>>> upstream-releases
pub mod errors {
    error_chain! {
        links {
            AudioIPC(::audioipc::errors::Error, ::audioipc::errors::ErrorKind);
        }
        foreign_links {
            Cubeb(::cubeb::Error);
            Io(::std::io::Error);
            Canceled(::futures::sync::oneshot::Canceled);
        }
    }
}

use errors::*;

struct ServerWrapper {
    core_thread: core::CoreThread,
    callback_thread: core::CoreThread,
}

fn run() -> Result<ServerWrapper> {
    trace!("Starting up cubeb audio server event loop thread...");

    let callback_thread = try!(
        core::spawn_thread("AudioIPC Callback RPC", || {
            match promote_current_thread_to_real_time(0, 48000) {
                Ok(_) => { }
                Err(_) => {
                    debug!("Failed to promote audio callback thread to real-time.");
                }
            }
            trace!("Starting up cubeb audio callback event loop thread...");
            Ok(())
        }).or_else(|e| {
            debug!(
                "Failed to start cubeb audio callback event loop thread: {:?}",
                e.description()
            );
            Err(e)
        })
    );

    let core_thread = try!(
        core::spawn_thread("AudioIPC Server RPC", move || Ok(())).or_else(|e| {
            debug!(
                "Failed to cubeb audio core event loop thread: {:?}",
                e.description()
            );
            Err(e)
        })
    );

    Ok(ServerWrapper {
        core_thread: core_thread,
        callback_thread: callback_thread,
    })
}

#[no_mangle]
pub extern "C" fn audioipc_server_start() -> *mut c_void {
    match run() {
        Ok(server) => Box::into_raw(Box::new(server)) as *mut _,
        Err(_) => ptr::null_mut() as *mut _,
    }
}

#[no_mangle]
pub extern "C" fn audioipc_server_new_client(p: *mut c_void) -> PlatformHandleType {
    let (wait_tx, wait_rx) = oneshot::channel();
    let wrapper: &ServerWrapper = unsafe { &*(p as *mut _) };

    let cb_remote = wrapper.callback_thread.remote();

    // We create a connected pair of anonymous IPC endpoints. One side
    // is registered with the reactor core, the other side is returned
    // to the caller.
    MessageStream::anonymous_ipc_pair()
        .and_then(|(sock1, sock2)| {
            // Spawn closure to run on same thread as reactor::Core
            // via remote handle.
            wrapper.core_thread.remote().spawn(|handle| {
                trace!("Incoming connection");
                sock2.into_tokio_ipc(handle)
                    .and_then(|sock| {
<<<<<<< HEAD
                        let transport = framed_with_fds(sock, Default::default());
                        rpc::bind_server(transport, server::CubebServer::new(cb_remote), handle);
||||||| merged common ancestors
                        let transport = framed_with_fds(sock, Default::default());
                        rpc::bind_server(transport, CubebServer::new(cb_remote), handle);
=======
                        let transport = framed_with_platformhandles(sock, Default::default());
                        rpc::bind_server(transport, server::CubebServer::new(cb_remote), handle);
>>>>>>> upstream-releases
                        Ok(())
                    }).map_err(|_| ())
                    // Notify waiting thread that sock2 has been registered.
                    .and_then(|_| wait_tx.send(()))
            });
            // Wait for notification that sock2 has been registered
            // with reactor::Core.
            let _ = wait_rx.wait();
<<<<<<< HEAD
            Ok(sock1.into_raw_fd())
        }).unwrap_or(-1)
||||||| merged common ancestors
            Ok(sock1.into_raw_fd())
        })
        .unwrap_or(-1)
=======
            Ok(PlatformHandle::from(sock1).as_raw())
        }).unwrap_or(-1isize as PlatformHandleType)
>>>>>>> upstream-releases
}

#[no_mangle]
pub extern "C" fn audioipc_server_stop(p: *mut c_void) {
    let wrapper = unsafe { Box::<ServerWrapper>::from_raw(p as *mut _) };
    drop(wrapper);
}
