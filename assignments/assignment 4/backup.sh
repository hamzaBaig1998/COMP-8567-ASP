#!/bin/bash

homeDirectory="$HOME/home/hamza"
incrementBackupDir="$HOME/home/backup/ib"
completeBackupDir="$HOME/home/backup/cb"
backupLgFileDir="$HOME/home/backup/backup.log"
timeStampLgFormat="%a %d %b %Y %I:%M:%S %p %Z"
timeStampBackupFormat="%Y-%m-%d %H:%M:%S %Z"
sleepTiming=120
cbfileNamePrefix=20001
ibFileNamePrefix=10001

mkdir -p "$completeBackupDir"
mkdir -p "$incrementBackupDir"
mkdir -p "$homeDirectory"

touch "$backupLgFileDir"

completeBackup() {
  find "$homeDirectory" -name "*.txt" -type f -print0 | tar czvf "$completeBackupDir/cb$cbfileNamePrefix.tar.gz" --null -T -
  echo "$1  cb$cbfileNamePrefix.tar.gz was created" >>"$backupLgFileDir"

  cbfileNamePrefix=$((cbfileNamePrefix + 1))
}

incrementBackup() {
  new_files=$(find "$homeDirectory" -type f -name "*.txt" -newermt "$2")
  if [ -n "$new_files" ]; then
    backup_file="ib$ibFileNamePrefix.tar.gz"
    tar -czf "$incrementBackupDir/$backup_file" $new_files
    echo "$1  $backup_file was created" >>"$backupLgFileDir"

    ibFileNamePrefix=$((ibFileNamePrefix + 1))
  else
    echo "$1  No changes - Incremental backup was not created" >>"$backupLgFileDir"
  fi
}

while :; do
  currentLogTimeFormat=$(TZ=EST date +"$timeStampLgFormat")
  currentTime=$(date +"$timeStampBackupFormat")
  completeBackup "$currentLogTimeFormat"
  sleep $sleepTiming


  for i in {1..3}; do
    currentLogTimeFormat=$(TZ=EST date +"$timeStampLgFormat")
    incrementBackup "$currentLogTimeFormat" "$currentTime"
    currentTime=$(date +"$timeStampBackupFormat")
    sleep $sleepTiming
  done
done &
