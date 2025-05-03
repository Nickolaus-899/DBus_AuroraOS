gdbus call -e -d com.system.configurationManager -o \
/com/system/configurationManager/Application/confManagerApplication1 -m \
com.system.configurationManager.Application.Configuration.ChangeConfiguration "TimeoutPhrase" "<'Hello!'>"

echo "Method was called"