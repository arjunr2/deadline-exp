#!/bin/bash
task_list=$(jq --raw-output '.tasks[] | "\(.exec) \(.runtime) \(.deadline) \(.period) \(.affinity)"' manifest.json)
echo $task_list

while IFS=$' ' read executable runtime deadline period affinity; do
  let "util = ($runtime * 100) / period"
  # Run task
  eval "chrt -d -T $runtime -D $deadline -P $period 0 ./$executable &"
  pid=$!
  # Set Affinity
  eval "echo $pid >> /sys/fs/cgroup/cpuset/$affinity/tasks"

  core_id=$(awk '{print $39}' /proc/$pid/stat)

  echo "[$executable, $pid], Util: $util, Core:$core_id, Affinity:$affinity"
  sleep 0.5
done <<< "$task_list"
