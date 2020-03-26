
import os
import shutil
import subprocess

BUILD_DIR = "build"
QT_DLL = ["Core", "Gui", "Widgets"]

def buildDir():
  return BUILD_DIR

def getQtDir():
  with open(buildDir() + "/dllinfo.txt") as f:
    path = f.read().replace("/lib/cmake/Qt5", "")
    return path

def ship(file):
  print("Copying", file)
  shutil.copy(file, "release")

def main():
  subprocess.run(["cmake", "--build", buildDir(), "--target", "ffmpeg-gui", "--config", "Release"])
  
  if os.path.exists("release"):
    shutil.rmtree("release")
    
  os.mkdir("release")
  
  ship(buildDir() + "/release/ffmpeg-gui.exe")

  qt_path = getQtDir()
  print("Qt directory is", qt_path)

  for lib in QT_DLL:
    ship(qt_path + "/bin/Qt5" + lib + ".dll")
	
if __name__ == "__main__":
  main()
