# Project README: T-Rex Chrome Dino Game on STM32F446RE NUCLEO-F446RE

## Overview

This project focuses on creating an embedded system version of the Google Chrome T-Rex Dino Game on the STM32F446RE NUCLEO-F446RE development board. It features a duel mode where players can control obstacles for the opponent using ultrasonic sensor gestures, enhancing the interactive experience.

## Features

- **Single and Duel Modes**: Play in classic mode or engage in a duel, controlling your opponent's obstacles.
- **Ultrasonic Sensor for Gesture Control**: Navigate obstacles in duel mode with hand gestures.
- **Dynamic Difficulty**: The game gets harder, with faster obstacles as time progresses.
- **Score Tracking**: Measures survival time to calculate scores.

## Hardware Requirements

- STM32F446RE NUCLEO-F446RE Development Board
- LCD Screen
- Ultrasonic sensor for gesture detection
- IR sensors for duel communication
- Buttons for obstacle selection (optional)

## Software Requirements

- Firmware for the STM32F446RE to manage game logic, sensor input, and display output.
- Provided code templates for easier development.

## Setup and Configuration

1. **Hardware Setup**: Connect the LCD, ultrasonic sensor, IR sensors, and buttons to the STM32F446RE.
2. **Firmware Installation**: Load the game code onto the STM32F446RE. Ensure hardware components are initialized.
3. **Duel Mode Configuration**: For duel play, set up two systems with IR sensors aligned for communication.

## Development Steps

- **Week 1**: Implement gesture detection with the ultrasonic sensor.
- **Week 2**: Integrate sensor input for in-game control.
- **Week 3**: Finalize game integration, adding score display and duel mode via IR.

## Contributing

Contributions are welcome. Fork the project, add your features, and submit a pull request for review. Open an issue first for significant changes.

## License

This project is under the MIT License. See LICENSE for more details.

## Acknowledgments

Thanks to Abderrahim Ayoub and David-Olivier for their contributions and the entire ELE3312 course team.
