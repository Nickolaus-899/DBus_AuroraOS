#!/bin/bash

DIR="$HOME/com.system.configurationManager/"
APP_NAME="confManagerApplication1.json"

PATH="$DIR$APP_NAME"

CONF='{
    "Timeout": 3,
    "TimeoutPhrase": "Hello world!"
}'

echo $CONF > $PATH

echo "Conf file was initialized at $PATH"

