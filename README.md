# Angle-of-Acquisition
This is the repository for the AoA (Angle of Acquisition) team's work throughout the semester. All files (source code and design files) relating to the AoA team and the Fibonacci Sphere can be found in this repository. Branches of this repository are meant for team members of the AoA team and their individual contributions to the project.

# Testing Documentation
The following media displays the testing that has gone into the designing, building, and programming of the Fibonacci Sphere and its respective circuitry.

![mount_attatched](https://github.com/user-attachments/assets/9ca97999-852f-45db-ac1f-009551ab5cdd)
Fig.1

Figure 1 shows the initial attatchment of the servo mount that supports the sphere and is meant to allow the sphere to both rotate and tilt.

![mount_nuts](https://github.com/user-attachments/assets/225d69a9-4b00-49d9-8e77-9771de8e9219)
Fig.2

Figure 2 shows how the servo mount is internally attached to the Fibonacci Sphere via nuts and bolts. Thread locker has been added to avoid loosening under vibration and/or shock.

![sphere_new_wires](https://github.com/user-attachments/assets/30f61770-2100-4d67-b063-75f68112a976)
Fig.3

It was soon realized that the original shielded wiring that was used (black wires shown in Fig.2) were not necessary and were only more difficult to work with (i.e. to solder) due to them being frayed wires rather than solid. Figure 3 shows the old wires switched out for new red/blue wires with a solid core that allowed for easier soldering and a lighter load on the servo and its mount.

![sphere_new_wires2](https://github.com/user-attachments/assets/6b7f7d26-9bee-462e-9ad8-94b351727305)
Fig.4

Figure 4 gives a closer look at the wires inside the sphere to show that all wires have been labelled so that they can be connected to their proper terminals. Red wires all need to go to V<sub>CC</sub> and the blue wires will later be labeled to indicate which channel on which of the four multiplexers they belong to. This is to make diagnosis easier in the future.
