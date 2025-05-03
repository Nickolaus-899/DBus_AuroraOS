gdbus call -e -d com.system.configurationManager -o \
/com/system/configurationManager/Application/confManagerApplication1 -m \
com.system.configurationManager.Application.Configuration.ChangeConfiguration "Timeout" "<uint32 2>"

echo "Method was called"