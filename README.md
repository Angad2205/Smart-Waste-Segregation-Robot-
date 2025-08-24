# Smart-Waste-Segregation-Robot-

Smart Waste Segregation Robot 🤖♻️
Overview

The Smart Waste Segregation Robot is an autonomous robotic system designed to detect, pick, and segregate waste into appropriate bins. The robot combines robotics, sensors, and AI to automate trash collection and sorting, helping reduce manual effort and improve recycling efficiency.

Features

Autonomous Navigation: The buggy moves automatically and detects trash using ultrasonic sensors placed at multiple angles.

Robotic Arm: Picks up trash and places it into a smart bin.

Smart Bin Detection: IR sensors confirm that the trash is placed correctly before release.

AI-Based Segregation: ESP32-CAM streams the trash image, and a pretrained AI model classifies the trash type (e.g., plastic, metal).

Servo-Controlled Sorting: Servo motors move the trash to the correct compartment based on AI classification.

Hardware Components

Arduino (Uno/Nano or compatible)

ESP32-CAM Module

Robotic Arm (servo-controlled)

Ultrasonic Sensors (HC-SR04 or similar)

IR Sensors for smart bin detection

Servo motors for sorting

Motor driver (L298N or similar)

Chassis/Buggy for autonomous movement

Software Components

Arduino IDE

Pretrained AI model for trash classification (runs on ESP32-CAM)

Serial communication between ESP32 and Arduino for trash type signal

How It Works

Trash Detection: Ultrasonic sensors detect trash at multiple angles. The robot aligns so the trash is centered.

Trash Pickup: Robotic arm lowers, grips the trash, and lifts it.

Smart Bin Placement: Trash is placed in the smart bin. IR sensors confirm correct placement.

AI Classification: ESP32-CAM captures live video and identifies trash type using a pretrained model.

Trash Sorting: Arduino receives trash type and moves servo motors to direct trash to the right bin.
