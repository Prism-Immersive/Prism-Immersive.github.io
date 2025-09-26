![license](https://img.shields.io/badge/Platform-Android-green "Android")
![license](https://img.shields.io/badge/Licence-Apache%202.0-blue.svg "Apache")

## Table of Contents
- [Introduction](#introduction)
- [Code Release](#code-release)
- [For Developers](#for-developers)

## Introduction
In this work, we present Prism, the first system designed to enable lossless immersive computing over consumer networks.
Emerging immersive workloads (e.g., medical simulation and industrial design) are executed remotely because large proprietary assets and sensitive data are impractical or undesirable to replicate widely. Yet, they must preserve fine visual detail—losing a thin vessel or sub-millimeter edge can compromise the accuracy or even validity of the results. The prevailing architectures render frames in the cloud and stream them as compressed video. To stay within bandwidth budgets of typical consumer networks, they have to rely on lossy compression that quantizes or blurs high-definition geometry, greatly impacting the QoS of fidelity-critical use cases.

To address this, we design and implement Prism, the first system that enables lossless immersive computing over consumer networks. We introduce a new architecture termed semantic GPU disaggregation: instead of rendering and streaming the frames, the cloud strategically offloads GPU rendering tasks to the client, completely avoiding lossy video compression. Specifically, Prism sends a reorganized, lightweight stream of low-level graphics commands and resource deltas to the client by exploiting latent structural regularities hidden in GPU command/resource semantics, and losslessly reconstruct the render pipeline on the client. With a practical bandwidth budget below 100 Mbps, Prism achieves substantially higher frame rates and lower latency compared to video streaming.

## Code Release
We have released the code for both server and client. In total, the core logic consists of 80k lines of C++ code.
The code is available at [Prism-Immersive/Prism-Immersive.github.io](https://github.com/Prism-Immersive/Prism-Immersive.github.io).

The code is organized as follows:
### Client
<table>
  <tr>
    <td>Directory</td>
    <td>Description</td>
    <td>Source Code</td>
  </tr>
  <tr>
    <td rowspan='6'>data</td>
    <td rowspan='6'>Data decompression framework implementing latent semantic restoration for GPU resources. Reconstructs compressed frame data, vertex arrays, and texture data from server-side compression while maintaining LRU caches for optimal memory management. Handles specialized decompression of floating-point vertex data, QOI texture format, and operation sequence differentials to minimize bandwidth usage and improve rendering performance across consumer network connections.</td>
    <td>Decompressor.h</td>
  </tr>
  <tr>
    <td>FrameDecompressor.cpp</td>
  </tr>
  <tr>
    <td>FrameDecompressor.h</td>
  </tr>
  <tr>
    <td>LRUCache.h</td>
  </tr>
  <tr>
    <td>ResourceDecompressor.cpp</td>
  </tr>
  <tr>
    <td>ResourceDecompressor.h</td>
  </tr>
  <tr>
    <td rowspan='17'>egl</td>
    <td rowspan='17'>Graphics Projection-based EGL implementation that maintains local context data and resource handles to avoid synchronous queries to remote server. Provides comprehensive translation interface between remote GPU operations and local graphics hardware while managing EGL contexts, surfaces, configurations, and synchronization primitives. This module ensures seamless integration with various GPU vendors and maintains compatibility across different graphics drivers and hardware platforms.</td>
    <td>Config.cpp</td>
  </tr>
  <tr>
    <td>Config.h</td>
  </tr>
  <tr>
    <td>Context.cpp</td>
  </tr>
  <tr>
    <td>Context.h</td>
  </tr>
  <tr>
    <td>Display.cpp</td>
  </tr>
  <tr>
    <td>Display.h</td>
  </tr>
  <tr>
    <td>EglTranslator.cpp</td>
  </tr>
  <tr>
    <td>EglTranslator.h</td>
  </tr>
  <tr>
    <td>Fence.cpp</td>
  </tr>
  <tr>
    <td>Fence.h</td>
  </tr>
  <tr>
    <td>FuncIDMap.cpp</td>
  </tr>
  <tr>
    <td>GPU.cpp</td>
  </tr>
  <tr>
    <td>GPU.h</td>
  </tr>
  <tr>
    <td>Surface.cpp</td>
  </tr>
  <tr>
    <td>Surface.h</td>
  </tr>
  <tr>
    <td>Sync.cpp</td>
  </tr>
  <tr>
    <td>Sync.h</td>
  </tr>
  <tr>
    <td rowspan='14'>network</td>
    <td rowspan='14'>Multi-channel network communication subsystem implementing dedicated transport channels per render thread with TCP and BBR congestion control. Receives GPU command streams from server, handles resource barrier synchronization for shared resource protection, and implements rate control feedback based on local GPU queue telemetry. This layer ensures ordered delivery of GPU operations while maintaining low latency and high throughput communication in the disaggregated GPU architecture.</td>
    <td>FlowController.cpp</td>
  </tr>
  <tr>
    <td>FlowController.h</td>
  </tr>
  <tr>
    <td>FrameCallPoller.cpp</td>
  </tr>
  <tr>
    <td>FrameCallPoller.h</td>
  </tr>
  <tr>
    <td>InputServiceProxy.cpp</td>
  </tr>
  <tr>
    <td>InputServiceProxy.h</td>
  </tr>
  <tr>
    <td>MainClient.cpp</td>
  </tr>
  <tr>
    <td>MainClient.h</td>
  </tr>
  <tr>
    <td>RemoteMemWriter.cpp</td>
  </tr>
  <tr>
    <td>RemoteMemWriter.h</td>
  </tr>
  <tr>
    <td>ServiceProxy.cpp</td>
  </tr>
  <tr>
    <td>ServiceProxy.h</td>
  </tr>
  <tr>
    <td>ThreadPacketPoller.cpp</td>
  </tr>
  <tr>
    <td>ThreadPacketPoller.h</td>
  </tr>
  <tr>
    <td rowspan='21'>opengl</td>
    <td rowspan='21'>OpenGL ES execution engine that processes GPU commands received from remote server while maintaining local graphics state consistency. Implements resource barrier protocol for shared resource coordination, handles concurrent dependency resolution through partial reordering, and executes batched rendering commands with pixel-perfect accuracy. This comprehensive module manages OpenGL contexts, shader programs, texture operations, vertex processing, and memory management to deliver lossless remote rendering without compression artifacts.</td>
    <td>Context.cpp</td>
  </tr>
  <tr>
    <td>Context.h</td>
  </tr>
  <tr>
    <td>FuncIDMap.cpp</td>
  </tr>
  <tr>
    <td>GLTranslator.cpp</td>
  </tr>
  <tr>
    <td>GLTranslator.h</td>
  </tr>
  <tr>
    <td>GLv1.cpp</td>
  </tr>
  <tr>
    <td>GLv1.h</td>
  </tr>
  <tr>
    <td>Helper.cpp</td>
  </tr>
  <tr>
    <td>Helper.h</td>
  </tr>
  <tr>
    <td>Memory.cpp</td>
  </tr>
  <tr>
    <td>Memory.h</td>
  </tr>
  <tr>
    <td>Program.cpp</td>
  </tr>
  <tr>
    <td>Program.h</td>
  </tr>
  <tr>
    <td>Resource.cpp</td>
  </tr>
  <tr>
    <td>Resource.h</td>
  </tr>
  <tr>
    <td>State.cpp</td>
  </tr>
  <tr>
    <td>State.h</td>
  </tr>
  <tr>
    <td>Texture.cpp</td>
  </tr>
  <tr>
    <td>Texture.h</td>
  </tr>
  <tr>
    <td>Vertex.cpp</td>
  </tr>
  <tr>
    <td>Vertex.h</td>
  </tr>
  <tr>
    <td rowspan='10'>renderer</td>
    <td rowspan='10'>Triple-buffering rendering pipeline coordinating between GPU processing threads and presentation threads. Manages graphics buffer allocation, event handling, and asynchronous GPU command execution with frame rate synchronization. Implements the client-side presentation surface management while GPU operations execute on dedicated render buffers, ensuring smooth visual output and responsiveness for immersive applications through optimized buffer swapping and synchronization mechanisms.</td>
    <td>Event.cpp</td>
  </tr>
  <tr>
    <td>Event.h</td>
  </tr>
  <tr>
    <td>GraphicBuffer.cpp</td>
  </tr>
  <tr>
    <td>GraphicBuffer.h</td>
  </tr>
  <tr>
    <td>MainController.cpp</td>
  </tr>
  <tr>
    <td>MainController.h</td>
  </tr>
  <tr>
    <td>MainRenderer.cpp</td>
  </tr>
  <tr>
    <td>MainRenderer.h</td>
  </tr>
  <tr>
    <td>RenderThread.cpp</td>
  </tr>
  <tr>
    <td>RenderThread.h</td>
  </tr>
  <tr>
    <td rowspan='2'>root</td>
    <td rowspan='2'>Application entry point and build system configuration containing the main client initialization code and CMake build scripts. The ClientMain.cpp serves as the primary coordinator that initializes all subsystems, manages application lifecycle, and orchestrates the interaction between network, rendering, and GPU translation components to deliver the complete cloud graphics experience.</td>
    <td>ClientMain.cpp</td>
  </tr>
  <tr>
    <td>CMakeLists.txt</td>
  </tr>
</table>

### Server

<table>
  <tr>
    <td>Directory</td>
    <td>Description</td>
    <td>Source Code</td>
  </tr>
  <tr>
    <td rowspan='12'>data</td>
    <td rowspan='12'>Latent semantic restoration framework implementing targeted compression through GPU data semantic reconstruction. Provides shader program precompilation and caching, FPZIP-based floating-point vertex compression with memory layout parsing, QOI texture compression for dynamic textures, and operation sequence differential compression using double-buffer caches. This module performs comprehensive data tracing and maintains LRU caches to minimize bandwidth usage while preserving lossless rendering quality.</td>
    <td>Cache.cpp</td>
  </tr>
  <tr>
    <td>Cache.h</td>
  </tr>
  <tr>
    <td>Compressor.h</td>
  </tr>
  <tr>
    <td>DataTracer.cpp</td>
  </tr>
  <tr>
    <td>DataTracer.h</td>
  </tr>
  <tr>
    <td>FrameCompressor.cpp</td>
  </tr>
  <tr>
    <td>FrameCompressor.h</td>
  </tr>
  <tr>
    <td>LRUCache.h</td>
  </tr>
  <tr>
    <td>ProgramCache.cpp</td>
  </tr>
  <tr>
    <td>ProgramCache.h</td>
  </tr>
  <tr>
    <td>ResourceCompressor.cpp</td>
  </tr>
  <tr>
    <td>ResourceCompressor.h</td>
  </tr>
  <tr>
    <td rowspan='25'>egl</td>
    <td rowspan='25'>Graphics Projection-based EGL implementation that intercepts EGL API calls from containerized Android applications while maintaining minimal context data and resource handles server-side. Manages EGL contexts, displays, surfaces, and GPU configurations with lifecycle management and state consistency across the disaggregated GPU architecture. This comprehensive module handles EGL function mapping, remote execution coordination, and synchronization primitives to enable efficient GPU operation forwarding to remote clients.</td>
    <td>Config.cpp</td>
  </tr>
  <tr>
    <td>Config.h</td>
  </tr>
  <tr>
    <td>Display.cpp</td>
  </tr>
  <tr>
    <td>Display.h</td>
  </tr>
  <tr>
    <td>EGL.cpp</td>
  </tr>
  <tr>
    <td>EglDef.h</td>
  </tr>
  <tr>
    <td>EglFunc.h</td>
  </tr>
  <tr>
    <td>EglFuncList.h</td>
  </tr>
  <tr>
    <td>EglImpl.cpp</td>
  </tr>
  <tr>
    <td>EglRemote.cpp</td>
  </tr>
  <tr>
    <td>EglUtils.cpp</td>
  </tr>
  <tr>
    <td>EglUtils.h</td>
  </tr>
  <tr>
    <td>Fence.cpp</td>
  </tr>
  <tr>
    <td>Fence.h</td>
  </tr>
  <tr>
    <td>FuncIDMap.cpp</td>
  </tr>
  <tr>
    <td>GPU.cpp</td>
  </tr>
  <tr>
    <td>GPU.h</td>
  </tr>
  <tr>
    <td>Lifecycle.cpp</td>
  </tr>
  <tr>
    <td>Lifecycle.h</td>
  </tr>
  <tr>
    <td>State.cpp</td>
  </tr>
  <tr>
    <td>State.h</td>
  </tr>
  <tr>
    <td>Surface.cpp</td>
  </tr>
  <tr>
    <td>Surface.h</td>
  </tr>
  <tr>
    <td>Sync.cpp</td>
  </tr>
  <tr>
    <td>Sync.h</td>
  </tr>
  <tr>
    <td rowspan='6'>include</td>
    <td rowspan='6'>Standard OpenGL ES and EGL header files organized by API version and extensions. Contains official Khronos Group headers for EGL, OpenGL ES 1.x/2.x/3.x, ETC1 texture compression, and KHR extensions. These headers provide the complete API definitions and constants required for GPU command interception, translation, and execution across different graphics API versions and vendor-specific extensions.</td>
    <td>EGL/</td>
  </tr>
  <tr>
    <td>ETC1/</td>
  </tr>
  <tr>
    <td>GLES/</td>
  </tr>
  <tr>
    <td>GLES2/</td>
  </tr>
  <tr>
    <td>GLES3/</td>
  </tr>
  <tr>
    <td>KHR/</td>
  </tr>
  <tr>
    <td rowspan='12'>network</td>
    <td rowspan='12'>Multi-channel network infrastructure implementing dedicated transport channels per render thread with concurrent dependency coordination. Manages resource barrier protocol through SetBarrier/WaitBarrier operations, implements Kingman queuing theory-based rate control with in-queue telemetry, and provides reliable TCP-based streaming with advanced flow control. This layer ensures efficient parallel transmission of GPU operation streams while resolving concurrency hazards and preventing client-side queue overload.</td>
    <td>Event.cpp</td>
  </tr>
  <tr>
    <td>Event.h</td>
  </tr>
  <tr>
    <td>FlowController.cpp</td>
  </tr>
  <tr>
    <td>FlowController.h</td>
  </tr>
  <tr>
    <td>FrameCallStreamer.cpp</td>
  </tr>
  <tr>
    <td>FrameCallStreamer.h</td>
  </tr>
  <tr>
    <td>MainServer.cpp</td>
  </tr>
  <tr>
    <td>MainServer.h</td>
  </tr>
  <tr>
    <td>RemoteMemMapper.cpp</td>
  </tr>
  <tr>
    <td>RemoteMemMapper.h</td>
  </tr>
  <tr>
    <td>ThreadPacketStreamer.cpp</td>
  </tr>
  <tr>
    <td>ThreadPacketStreamer.h</td>
  </tr>
  <tr>
    <td rowspan='30'>opengl</td>
    <td rowspan='30'>Comprehensive OpenGL ES interception engine implementing concurrent dependency coordination with partial reordering for shared resource protection. Intercepts and processes all OpenGL API calls from containerized Android applications, maintains complete rendering state consistency, and coordinates resource barrier operations across multiple rendering contexts. This extensive module provides the core GPU disaggregation functionality through semantic understanding of GPU operations, efficient batched command streaming, and relaxed consistency model exploitation for maximized parallel execution.</td>
    <td>Compiler.cpp</td>
  </tr>
  <tr>
    <td>Compiler.h</td>
  </tr>
  <tr>
    <td>ErrorHandler.h</td>
  </tr>
  <tr>
    <td>ExtFunc.h</td>
  </tr>
  <tr>
    <td>Framebuffer.cpp</td>
  </tr>
  <tr>
    <td>Framebuffer.h</td>
  </tr>
  <tr>
    <td>FuncIDMap.cpp</td>
  </tr>
  <tr>
    <td>GL.cpp</td>
  </tr>
  <tr>
    <td>GLDef.h</td>
  </tr>
  <tr>
    <td>GLFuncList.h</td>
  </tr>
  <tr>
    <td>GLImpl.cpp</td>
  </tr>
  <tr>
    <td>GLRemote.cpp</td>
  </tr>
  <tr>
    <td>GLUtils.cpp</td>
  </tr>
  <tr>
    <td>GLUtils.h</td>
  </tr>
  <tr>
    <td>GLv1.cpp</td>
  </tr>
  <tr>
    <td>GLv1.h</td>
  </tr>
  <tr>
    <td>Loader.cpp</td>
  </tr>
  <tr>
    <td>Loader.h</td>
  </tr>
  <tr>
    <td>Memory.cpp</td>
  </tr>
  <tr>
    <td>Memory.h</td>
  </tr>
  <tr>
    <td>Program.cpp</td>
  </tr>
  <tr>
    <td>Program.h</td>
  </tr>
  <tr>
    <td>Resource.cpp</td>
  </tr>
  <tr>
    <td>Resource.h</td>
  </tr>
  <tr>
    <td>State.cpp</td>
  </tr>
  <tr>
    <td>State.h</td>
  </tr>
  <tr>
    <td>Texture.cpp</td>
  </tr>
  <tr>
    <td>Texture.h</td>
  </tr>
  <tr>
    <td>Vertex.cpp</td>
  </tr>
  <tr>
    <td>Vertex.h</td>
  </tr>
  <tr>
    <td rowspan='4'>root</td>
    <td rowspan='4'>Server application configuration and asynchronous threading infrastructure with Android container integration. The ThreadContext module provides multi-threaded GPU command interception and processing coordination, managing asynchronous operation buffering, periodic network flushing, and rate-controlled execution pacing. This fundamental execution environment enables seamless GPU disaggregation between containerized Android applications and remote client devices in cloud environments.</td>
    <td>Android.bp</td>
  </tr>

  <tr>
    <td>CMakeLists.txt</td>
  </tr>
  <tr>
    <td>ThreadContext.cpp</td>
  </tr>
  <tr>
    <td>ThreadContext.h</td>
  </tr>
</table>

## For Developers

### Implementation Overview
Prism  is designed as a userspace GPU driver library that exposes standard OpenGL ES interfaces for compatibility with existing immersive applications.

### Server Deployment
The server runs containerized Android applications on x86-based Linux servers using Docker for filesystem isolation. We utilize Intel Houdini binary translator for ARM compatibility and bypass Android's system-wide display composition for optimal performance. The server captures only foreground application GPU operations, discarding other system rendering instructions to minimize overhead.

### Client Integration
The client operates as a standard userspace application implementing triple-buffering architecture. GPU operations execute on dedicated render buffers while a separate presentation thread manages the display surface. This design ensures smooth frame presentation and pipeline resilience against network fluctuations.

### Network Configuration
Prism requires reliable TCP transport with BBR congestion control, as GPU operations cannot be dropped without corrupting the rendering pipeline state. Each render thread utilizes a dedicated network channel for parallel command transmission while maintaining operation ordering through resource barrier protocols.

### System Requirements
- **Server**: x86-64 Linux with Docker support, containerized Android runtime
- **Client**: OpenGL ES 2.0/3.0 compatible GPU, TCP network connectivity
- **Network**: Stable TCP connection preferred for optimal experience

### License and Usage
This codebase is distributed under Apache 2.0 license in accordance with AOSP licensing requirements. Please ensure compliance with open source policies when implementing modifications or commercial deployments.