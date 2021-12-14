# mazuMedical

Presented originally as a smart baby monitor that can estimate heart rate and respond to a low heart rate / no heart rate scenario to warn parents.

C++ & Qt application to perform eulerian video filtering (color magnification for heart rate estimation) on one Raspberry Pi and send video feed to another Pi.

One Raspberry Pi (server) acts as a camera and performs euclidean video magnification to estimate heart rate. The results are sent (sctp) to the other Raspberry Pi (client) which displays the video along with the heart rate estimation. 

[link to poster](./resources/imgs/MazuMedicalPoster.pdf)

### Algorithm Overview
![Algorithm overview](./resources/imgs/Poster_Camera_unit.png)

### TODOs
- [ ] separate image capture and server's frame buffer maintenance from algorithm implementation
- [ ] add additional algorithms for
  - [ ] breathing rate detection
  - [ ] movement
  - [ ] ...
- [ ] Hardware, from the ground up 
  1. [ ] move from raspberry pi to STM32 mcu (openMV -- H7)
  2. [ ] move from openMV to STM32 Nucleo/discovery board and purchased image sensor (AR0134CS) -- Image sensor I really want (OV2312).
  3. [ ] Find manufacturer for production run.

