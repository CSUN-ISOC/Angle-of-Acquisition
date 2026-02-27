# Angle-of-Acquisition
This is the repository for the AoA (Angle of Acquisition) team's work throughout the semester. All files (source code and design files) relating to the AoA team and the Fibonacci Sphere can be found in this repository. Branches of this repository are meant for team members of the AoA team and their individual contributions to the project.

The primary goal of the AoA team this semester is to improve the design of the sphere and to give it the ability to adjust its positioning depending on the angle of an incoming modulated signal from a laser diode.


# Simulations
<img width="603" height="541" alt="Screenshot 2026-02-26 180515" src="https://github.com/user-attachments/assets/fd0ebe46-a909-49a5-83aa-9cc292405326" />

#### Fig.1



Figure 1 shows the design of the 3D-printed servo mount in CAD. This servo will allow the sphere to rotate 360 degrees and tilt 15 degrees in each direction.

<img width="761" height="683" alt="Screenshot 2026-02-26 180840" src="https://github.com/user-attachments/assets/c7919347-2b46-4893-871f-660c3fe589c9" />


#### Fig.2


Figure 2 shows a MATLAB simulation confirming proper operation of the entire Angle of Acquisition system. In this simulation, a theoretical flashlight is shined at the Fibonacci Sphere. The Fibonacci Sphere can identify where on the sphere the beam of light is shining by using data from all 64 of the photoresistors mounted on the sphere. From this, the sphere positions itself to make the beam of light shine on a receiver mounted to the exterior surface of the Fibonacci Sphere and thus allow the receiver to "focus" on the incoming data encoded into light source. The position of the flashlight changes every four seconds, and within those four seconds, the sphere is expected to move accordingly to line up the receiver to the beam of light. The square waves show the different positions of the light and the decaying graphs show the initial large error, which approaches zero in less than one second, meaning that the sphere properly identified the position of the incoming light and positioned the receiver as intended. This quick adjustment however, raised concerns about the forces that would result on the sphere, so the repositioning speed of the sphere was adjusted to be much slower. Even considering this adjustment, the sphere still has enough time to position itself and receive all of the incoming data.

<img width="475" height="247" alt="Screenshot 2026-02-26 182515" src="https://github.com/user-attachments/assets/5d58048f-20b8-4d63-9526-a95974ccf907" />


#### Fig.3


Figure 3 shows the design file for one quarter of the sphere. Due to its size, the Fibonacci Sphere had to be printed in quarters and attached together after the fact.


# Testing Documentation
The following media displays the testing that has gone into the designing, building, and programming of the Fibonacci Sphere and its respective circuitry.

<img width="447" height="567" alt="Screenshot 2026-02-26 182832" src="https://github.com/user-attachments/assets/0d87acf6-5466-49e6-9e86-a648c3d21fa9" />

#### Fig.1



Figure 1 shows the initial attatchment of the servo mount that supports the sphere and is meant to allow the sphere to both rotate and tilt.

<img width="428" height="548" alt="Screenshot 2026-02-26 182844" src="https://github.com/user-attachments/assets/38a0b988-a336-4232-abae-af3d28d84d9b" />

#### Fig.2


Figure 2 shows how the servo mount is internally attached to the Fibonacci Sphere via nuts and bolts. Thread locker has been added to avoid loosening under vibration and/or shock.

<img width="358" height="788" alt="Screenshot 2026-02-26 182904" src="https://github.com/user-attachments/assets/c6fb8c78-c498-452e-993d-42318e2f91ce" />

#### Fig.3


It was soon realized that the original shielded wiring that was used (black wires shown in Fig.2) were not necessary and were only more difficult to work with (i.e. to solder) due to them being frayed wires rather than solid. Figure 3 shows the old wires switched out for new red/blue wires with a solid core that allowed for easier soldering and a lighter load on the servo and its mount.

<img width="701" height="845" alt="Screenshot 2026-02-26 182914" src="https://github.com/user-attachments/assets/0703968f-bf8a-41b5-89ea-3853659b5539" />

#### Fig.4


Figure 4 gives a closer look at the wires inside the sphere to show that all wires have been labelled so that they can be connected to their proper terminals. Red wires all need to go to V<sub>CC</sub> and the blue wires will later be labeled to indicate which channel on which of the four multiplexers they belong to. This is to make diagnosis easier in the future.
