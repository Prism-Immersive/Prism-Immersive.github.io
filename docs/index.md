![license](https://img.shields.io/badge/Platform-Android-green "Android")
![license](https://img.shields.io/badge/Licence-Apache%202.0-blue.svg "Apache")

## Table of Contents
- [Introduction](#introduction)
- [Code Release](#code-release)

## Introduction
In this work, we present Prism, the first system designed to enable lossless immersive computing over consumer networks.
Emerging immersive workloads (e.g., medical simulation and industrial design) are executed remotely because large proprietary assets and sensitive data are impractical or undesirable to replicate widely. Yet, they must preserve fine visual detail—losing a thin vessel or sub-millimeter edge can compromise the accuracy or even validity of the results. The prevailing architectures render frames in the cloud and stream them as compressed video. To stay within bandwidth budgets of typical consumer networks, they have to rely on lossy compression that quantizes or blurs high-definition geometry, greatly impacting the QoS of fidelity-critical use cases.

To address this, we design and implement Prism, the first system that enables lossless immersive computing over consumer networks. We introduce a new architecture termed semantic GPU disaggregation: instead of rendering and streaming the frames, the cloud strategically offloads GPU rendering tasks to the client, completely avoiding lossy video compression. Specifically, Prism sends a reorganized, lightweight stream of low-level graphics commands and resource deltas to the client by exploiting latent structural regularities hidden in GPU command/resource semantics, and losslessly reconstruct the render pipeline on the client. With a practical bandwidth budget below 100 Mbps, Prism achieves substantially higher frame rates and lower latency compared to video streaming.

## Code Release
We have released the code for both server and client. In total, the core logic consists of 80k lines of C++ code.
The code is available at [Prism-Immersive/Prism-Immersive.github.io](https://github.com/Prism-Immersive/Prism-Immersive.github.io).