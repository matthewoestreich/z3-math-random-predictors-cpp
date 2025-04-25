# JSRandomnessPredictor

Predict JavaScript `Math.random()` output in V8 (Node.js), Chrome, and Firefox, from C++.

## Setup

1. **Install toolchain**:
   - Ensure the following tools are installed:
     - `cmake`
     - `vcpkg`
     - `make`

2. **Configure the build system and install dependencies**:
   - From the project root, run:
     ```bash
     cmake -B build
     ```
     This will configure the build system and automatically install dependencies via `vcpkg`.

3. **Build the project**:
   - Once dependencies are set up, compile the library by running:
     ```bash
     make -C build
     ```

4. **Run tests**:
   - After building, run tests to ensure everything works as expected:
     ```bash
     make -C build test
     ```
     This will execute unit tests (using `Catch2`) and display the results in the console.
