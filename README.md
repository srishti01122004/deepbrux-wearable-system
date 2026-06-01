# DeepBrux: Piezoelectric Wearable System for Bruxism Detection

> **IEEE SMC 2026 (USA) — manuscript under review**  
> Srishti Srivastava · Amit Bhongade · Prof. Tapan K. Gandhi (IIT Delhi, Dept. of Electrical Engineering)

---

## Overview

DeepBrux is an end-to-end wearable system for real-time bruxism (jaw-clenching) detection, built from the ground up — from transducer selection and analogue front-end circuit design through embedded firmware, on-device signal processing, and deep learning classification.

The system was validated on **8 human subjects** across **6 jaw-movement activity classes**, achieving **86% classification accuracy** with a ResNet-18 model trained on spectrograms derived from a custom-annotated physiological dataset.

This is a sole-designed hardware + firmware project, not a software-only implementation. Every layer — sensor, AFE, firmware pipeline, dataset collection — was built and validated independently.

---

## Hardware Design

| Component | Detail |
|---|---|
| Microcontroller | Teensy 4.1 (ARM Cortex-M7, 600 MHz) |
| Sensor | Piezoelectric transducer (selected for jaw-surface contact mechanics and sub-mV signal sensitivity) |
| Sensor interface | Direct ADC (piezoelectric → AFE → Teensy ADC pin) |
| Data logging | SD card over SPI |
| Acquisition rate | 200 Hz continuous, interrupt-driven |
| Analogue front-end | Custom signal conditioning circuit: amplification, noise filtering, impedance matching for piezoelectric source |
| Power | Low-power design for wearable form factor |

The analogue front-end was designed from scratch — piezoelectric transducers present high source impedance and generate sub-mV signals that require careful conditioning before digitisation. The AFE handles amplification, high-pass filtering for baseline removal, and anti-aliasing before ADC sampling.

---
<img width="551" height="253" alt="FIG1" src="https://github.com/user-attachments/assets/96ed680f-5f59-4b49-9d46-fd706c67449b" />


## Firmware Architecture


**4-stage on-device pipeline:**

```
Raw piezo signal (200 Hz)
        │
        ▼
[Stage 1] bandpass and notch Filtering
        Noise suppression + baseline wander removal
        │
        ▼
[Stage 2] Segmentation
        Fixed-window activity segmentation
        │
        ▼
[Stage 3] Spectrogram Conversion
        Time-frequency feature representation
        │
        ▼
[Stage 4] Data Augmentation
        On-device augmentation for dataset robustness
        │
        ▼
Annotated dataset → ML training pipeline
```

Key firmware details:
- ISR-driven ADC sampling with hardware timer — no polling, deterministic 200 Hz latency
- Timestamped data logging to SD card over SPI for reproducible dataset annotation
- Validated for timing jitter across extended recording sessions

---

## Machine Learning Pipeline

| Stage | Detail |
|---|---|
| Feature representation | Short-time Fourier Transform (STFT) spectrograms |
| Model | ResNet-18 (transfer learning, fine-tuned) |
| Classes | 6 jaw-movement activity classes |
| Subjects | 8 human subjects |
| Accuracy | **86%** |
| Dataset | Custom-annotated; collected using DeepBrux hardware |

The dataset was collected entirely using this hardware system — no existing public dataset was used. Annotation was done manually with ground-truth activity labels, making this a clinical-grade data collection pipeline.

---

## System Architecture

```
┌─────────────────────────────────────────────────────┐
│                   DeepBrux Hardware                 │
│                                                     │
│  Piezo Sensor → AFE Circuit → Teensy 4.1 (200 Hz)  │
│                               │                     │
│                               │
│                               │                     │   │
└───────────────────────────────┼─────────────────────┘
                                │
                    Annotated Dataset (custom)
                                │
                    ┌───────────▼──────────┐
                    │   ResNet-18 Classifier│
                    │   86% accuracy        │
                    │   6 activity classes  │
                    └──────────────────────┘
```

---

## Results

- **86% classification accuracy** across 6 jaw-movement classes
- Validated on 8 human subjects under controlled conditions
- Dataset: custom-annotated, collected using DeepBrux hardware
- Confusion matrix and per-class accuracy: see `results/`

---

## Publication

> Srishti Srivastava, Amit Bhongade, Tapan K. Gandhi —
> **"DeepBrux: Piezoelectric-Based Wearable System for Bruxism Detection Using Deep Learning"**
> *IEEE SMC 2026 (USA) — under review*

---

## Background: What is Bruxism?

Bruxism is a condition characterised by involuntary jaw clenching and teeth grinding, often during sleep. It affects ~8–31% of the population and can cause significant dental damage, jaw pain, and headaches. Clinical detection currently requires overnight polysomnography in a lab setting — there is no validated wearable solution. DeepBrux aims to enable ambulatory, low-cost bruxism monitoring.

---

## Built At

**IIT Delhi — Dept. of Electrical Engineering**  
Mentor: Prof. Tapan K. Gandhi  
May – July 2025

---

## Contact

Srishti Srivastava — [srishtisrivastava2004@gmail.com](mailto:srishtisrivastava2004@gmail.com)  
[LinkedIn](https://linkedin.com/in/srishti-srivastava-2b910124b) · [GitHub](https://github.com/srishti01122004)
