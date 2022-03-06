<div id="top"></div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

A simple 32 bit hobby OS made in order to learn more about how CPUs and OSes work.

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [GCC](https://gcc.gnu.org/)
* [QEMU](https://qemu.org/)
* [GNU Make](https://gnu.org/software/make/)
* [GDB](https://www.sourceware.org/gdb/)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

* GNU make
  ```sh
  sudo apt install make
  ```
* GCC
  ```sh
  sudo apt install gcc
  ```
* QEMU
  ```sh
  sudo apt install qemu-system
  ```
* GDB (optional)
  ```
  sudo apt install gdb
  ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

* To emulate the OS in qemu:
  ```sh
  make
  ```
* To write the OS to /dev/sda **ENSURE THAT /dev/sda (or whatever you have changed this to in the makefile) is not a system critical disk**:
  ```sh
  make install
  ```
  then enter your password when prompted
* To debug the OS:
  ```sh
  make debug
  ```
  then in a separate shell:
  ```sh
  bash gdb.sh
  ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [x] Basic boot and hello world
- [x] Switch to 32 bit
- [x] VESA support for higher resolutions
- [ ] Virtual memory
- [ ] Advanced hardware detection
    - [ ] PCI
    - [ ] ACPI
- [ ] Basic power management
- [ ] Reading from secondary storage
- [ ] Basic filesystem support
- [ ] Multiprocessing
- [ ] IPC
- [ ] Multithreading

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Please note that this is a hobby project and is not even fully featured enough to turn off yet

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Project Link: [https://github.com/MarkRoss470/pepperOS](https://github.com/MarkRoss470/pepperOS)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [osdev.org wiki](https://wiki.osdev.org)

<p align="right">(<a href="#top">back to top</a>)</p>
