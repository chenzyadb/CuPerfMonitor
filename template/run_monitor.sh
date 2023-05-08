#!/system/bin/sh
BASE_DIR=$(dirname "$0")
${BASE_DIR}/perf_monitor -R ${BASE_DIR}/config.json ${BASE_DIR}/output.csv
