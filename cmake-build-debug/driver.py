#!/usr/bin/env python

import sys
import os
from time import sleep
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

filename = ''


class Handler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path == "./" + filename:
            # in this example, we only care about this one file
           os.system("ftex " + filename)


if len(sys.argv) > 0:
    filename = sys.argv[1]
else:
    raise Exception("Please provide a filename!")

observer = Observer()
observer.schedule(Handler(), ".")  # watch the local directory
observer.start()

try:
    while True:
        sleep(1)
except KeyboardInterrupt:
    observer.stop()

observer.join()