#!/bin/bash

if [ `sudo service hyperion status | grep running | wc -l` = 1 ]
then
        curl -v -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"id":1,"jsonrpc":"2.0","method":"GUI.ShowNotification","params":{"title":"Hyperion","message":"Disabled","image":"/home/osmc/hyperion.png"}}' http://127.0.0.1:8080/jsonrpc
        sudo service hyperion stop

else
        curl -v -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"id":1,"jsonrpc":"2.0","method":"GUI.ShowNotification","params":{"title":"Hyperion","message":"Enabled","image":"/home/osmc/hyperion.png"}}' http://127.0.0.1:8080/jsonrpc
        sudo service hyperion start
fi
