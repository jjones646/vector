# Vector

A custom implementation of std::vector for showing how the container runs in memory.


## Usage

Use the `make` command to compile the program.
```
make
```

Then you can run the program and check the results for accuracy.
```
# Run the program & log the output
./testVec >> output-results.txt

# Check against the minimum expected results
vimdiff testVec-expected-output.txt output-results.txt
```
