# !/bin/bash

start_time=$(date +%s)

#./test_prog
./bin/fuzz_mj_mut

end_time=$(date +%s)
run_time=$(($end_time-$start_time))
echo "it took $run_time"


