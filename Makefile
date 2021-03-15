all:						buildandlaunch

buildandlaunch:				
						docker build -t test_container .
						python -m webbrowser "http://127.0.0.1:6080/#/?video"
						docker run -it --rm -p 6080:80 --device /dev/snd test_container


.PHONY: all buildandlaunch