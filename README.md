[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WXeqVgks)
# final-project-skeleton

* Team Number: 11
* Team Name: Circuit Breakers
* Team Members: Eshan Singhal, Mohit Shah, Nandini Swami
* GitHub Repository URL: https://github.com/upenn-embedded/final-project-s25-the-circuit-breakers
* GitHub Pages Website URL: [for final submission]

## Final Project Proposal

### 1. Abstract

MicroBeat is an interactive at-home music production system that allows users to create and manipulate sounds using various input sensors. The system integrates pressure sensors for drum simulation, a potentiometer for volume control, and an accelerometer for dynamic sound modulation. The DFPlayer Mini MP3 module handles audio output, enabling seamless playback through a connected speaker. By combining these components, MicroBeat transforms user interactions into a live DJ-style music experience, making music production more accessible and engaging.

MicroBeat is an at-home, music production system that enables anyone with a passion to make music to make unique beats. 

### 2. Motivation

*What is the problem that you are trying to solve? Why is this project interesting? What is the intended purpose?*

Traditional DJ sets contain a multitude of complex components with turntables, mixers, and MIDI components that require a relatively large learning curve. Additionally, this static setup can be limiting in terms of expressive control which constrains creativity.

This project is very interesting because we ideate and develop a more expressive, intuitive medium to control music with various complex sensors. The intended purpose is to enable more complex DJing while increasing accessibility.

### 3. System Block Diagram

Hardware Design:

![](blockdiagram.png)

Software Design:



### 4. Design Sketches

The project will consist of a module that houses the ATmega328PB, speakers, and other core electronic components. There will be a separate component with various drum pads using pressure sensors as well as wristband with the accelerometer/gyroscope.

Critical design features?

There will be some special manufacturing techniques required such as 3D printing/laser cutting to create housing for the electronics, 3D printing to create the drum pads, and 3D printing to create the wristband for the accelerometer/gyroscope.


![](finalProjDesign.jpeg)

We will need to 3D print a box to place the three subsystems of our project. On the base of the box, the turntable will be printed onto the base and the pads are placed to the right. Platforms will be placed above the pressure sensors and the circuitry will be placed inside the box (breadboard + wiring for the sensors). The accelerator is tied to the wrist using a band for use in simulating the turntable.

### 5. Software Requirements Specification (SRS)

*Formulate key software requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**5.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

**5.2 Functionality**

| ID     | Description                                                                                                                                                                                                              |
| ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| SRS-01 | Real time sensor input processing: System must accurately detect pressure changes (drums), potentiometer adjustments (note change), and accelerometer motion to trigger corresponding audio responses. To validate this we can use an oscilloscope or logic analyzer to verify signal integrity and response time from sensors to the ATmega328’s input pins.                                                                                                                |
| SRS-02 | Communication between AtMega and DFP Module: The system must send UART commands to the DFPlayer Mini MP3 module with minimal delay to ensure immediate sound output upon input detection. We can validate this by measuring delay between input activation and audio playback using timestamp logging.                                                                                                                                         |
| SRS-03 | ADC Input handling: The potentiometer (volume control) and pressure sensors must be read via ADC, mapped to corresponding levels, and converted into digital signals for audio processing. We can validate this by comparing raw ADC values with expected sensor outputs to ensure accurate readings within ±5% error margin. |
| SRS-04 | Piano note generation + Pitch control (PWM): The system must detect button presses and assign the corresponding octave by setting a predefined frequency range. The system must read the potentiometer's ADC value and map it to specific note frequencies within the selected octave. The system must use Timer1 (OCR1A register) to generate a PWM square wave at the required frequency based on the selected octave and note. Use an oscilloscope to verify correct frequency output for each button press and potentiometer adjustment.                                                                                |
| SRS-05 | Turntable movement detection: The system should be able to detect certain shapes from the accelerometer/gyroscope setup such as a circular motion, shaking, and vertical movement. We can test this functionality by rigorously testing the setup in a real environment to determine if the desired shape is being recorded. |

### 6. Hardware Requirements Specification (HRS)

*Formulate key hardware requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**6.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

**6.2 Functionality**

| ID     | Description                                                                                                                        |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | The pressure sensors shall carry a maximum load of 1 kg of applied force and measurements above this threshold shall saturate to 1 kg. Connected to a speaker, drum pads attached to the sensors shall mimic sounds of a hi-hat, snare, and bass drum. ADC readings shall change the volume of these sounds according to the pressure applied. |
| HRS-02 | The potentiometer shall be set at one of 8 notes in the current octave, depending on the amount turned. A max voltage of 5 V shall be measured at the nodes of the potentiometer before input to the ADC. 8 concurrent voltage ranges of step size 0.4 V shall be used to differentiate between each note.                                                           |
| HRS-03 | Speakers shall be used to output preloaded music and signals sent from the piano and drum pads. This shall be validated through using the DFPlayer module where 30 module levels dictate the volume at which the signal is sent at.                      |
| HRS-04 | The IMU should have a 500 Hz sampling rate because it should be sufficient to detect most fast hand motions. We can test if this condition is met by sampling for 1 second and confirming the number of recorded samples is 500. |
| HRS-05 | The IMU should also have accurate relative position and rotation accuracy. This can be tested by making various hand motions and confirming that the motion is being accurately represented by the IMU via 3D plotting. |

### 7. Bill of Materials (BOM)

*What major components do you need and why? Try to be as specific as possible. Your Hardware & Software Requirements Specifications should inform your component choices.*

*In addition to this written response, copy the Final Project BOM Google Sheet and fill it out with your critical components (think: processors, sensors, actuators). Include the link to your BOM in this section.*

The components we need include 3 piezo pressure sensors to represent the snare, hi-hat, and the bass drum. The preferred type had a range of 0 lbs to 4.4 lbs., as nothing more than the max was needed due to continuous playing (taps). The DFPlayer Mini allows for manipulation with prerecorded audio and speaker output. The accelerometer is crucial as it changes audio according to circular or vertical motion of the device attached to the wrist, simulating a turntable.

[Final Project BOM Link](https://docs.google.com/spreadsheets/d/1Nn4dQrrRypU64VvPleaDI948HICvbxTs2iRi9GGM9Bo/edit?usp=sharing)

### 8. Final Demo Goals

*How will you demonstrate your device on demo day? Will it be strapped to a person, mounted on a bicycle, require outdoor space? Think of any physical, temporal, and other constraints that could affect your planning.*

We will demo the device by playing a preloaded song and individually demonstrating the functionality of each subsystem and explain the underlying hardware and software implementation and conclude by using all components simultaneously. Additionally, the accelerometer will be strapped to the wrist when controlling speed of the song. The piano will be tested separately by showing different notes by twisting the potentiometer and pressing buttons to change octave

### 9. Sprint Planning

*You've got limited time to get this project done! How will you plan your sprint milestones? How will you distribute the work within your team? Review the schedule in the final project manual for exact dates.*

| Milestone  | Functionality Achieved | Distribution of Work |
| ---------- | ---------------------- | -------------------- |
| Sprint #1  |  Source all the hardware and plan the structure and exact functionality of software and build CAD                      |  Eshan: Turntable/Sound morpher, Mohit: Drum pads, Nandini: Piano                    |
| Sprint #2  | Build software and generate cohesive working prototype                       |  Eshan: Turntable/Sound morpher, Mohit: Drum pads, Nandini: Piano                    |
| MVP Demo   |  Work through integration bugs and flow                      | Working together to solve issues                     |
| Final Demo |   Rigorously test and enable additional functionality (if possible)                     |   Working together to solve issues                   |

**This is the end of the Project Proposal section. The remaining sections will be filled out based on the milestone schedule.**

## Sprint Review #1

### Last week's progress

### Current state of project

### Next week's plan

## Sprint Review #2

### Last week's progress

### Current state of project

### Next week's plan

## MVP Demo

1. Show a system block diagram & explain the hardware implementation.
2. Explain your firmware implementation, including application logic and critical drivers you've written.
3. Demo your device.
4. Have you achieved some or all of your Software Requirements Specification (SRS)?

   1. Show how you collected data and the outcomes.
5. Have you achieved some or all of your Hardware Requirements Specification (HRS)?

   1. Show how you collected data and the outcomes.
6. Show off the remaining elements that will make your project whole: mechanical casework, supporting graphical user interface (GUI), web portal, etc.
7. What is the riskiest part remaining of your project?

   1. How do you plan to de-risk this?
8. What questions or help do you need from the teaching team?

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a GitHub pages website before, you can follow this webpage (though, substitute your final project repository for the GitHub username one in the quickstart guide):  [https://docs.github.com/en/pages/quickstart](https://docs.github.com/en/pages/quickstart)

### 1. Video

[Insert final project video here]

* The video must demonstrate your key functionality.
* The video must be 5 minutes or less.
* Ensure your video link is accessible to the teaching team. Unlisted YouTube videos or Google Drive uploads with SEAS account access work well.
* Points will be removed if the audio quality is poor - say, if you filmed your video in a noisy electrical engineering lab.

### 2. Images

[Insert final project images here]

*Include photos of your device from a few angles. If you have a casework, show both the exterior and interior (where the good EE bits are!).*

### 3. Results

*What were your results? Namely, what was the final solution/design to your problem?*

#### 3.1 Software Requirements Specification (SRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                                               | Validation Outcome                                                                          |
| ------ | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| SRS-01 | The IMU 3-axis acceleration will be measured with 16-bit depth every 100 milliseconds +/-10 milliseconds. | Confirmed, logged output from the MCU is saved to "validation" folder in GitHub repository. |

#### 3.2 Hardware Requirements Specification (HRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                                                                        | Validation Outcome                                                                                                      |
| ------ | ---------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | A distance sensor shall be used for obstacle detection. The sensor shall detect obstacles at a maximum distance of at least 10 cm. | Confirmed, sensed obstacles up to 15cm. Video in "validation" folder, shows tape measure and logged output to terminal. |
|        |                                                                                                                                    |                                                                                                                         |

### 4. Conclusion

Reflect on your project. Some questions to address:

* What did you learn from it?
* What went well?
* What accomplishments are you proud of?
* What did you learn/gain from this experience?
* Did you have to change your approach?
* What could have been done differently?
* Did you encounter obstacles that you didn’t anticipate?
* What could be a next step for this project?

## References

Fill in your references here as you work on your final project. Describe any libraries used here.
