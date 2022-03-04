from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import sys

if len(sys.argv) == 1:
    raise ValueError('please pass a file to watch.')

else:


