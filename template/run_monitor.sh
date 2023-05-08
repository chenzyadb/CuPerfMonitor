#!/system/bin/sh
BASE_DIR=$(dirname "$0")

chmod 0777 ${BASE_DIR}/perf_monitor
${BASE_DIR}/perf_monitor -R ${BASE_DIR}/config.json ${BASE_DIR}/output.csv
