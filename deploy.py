
import glob
import os
import shutil
import subprocess

BUILD_DIR = "build"
QT_DLL = ["Core", "Gui", "Widgets", "Svg"]
QT_PLUGINS = ["platforms"]

def buildDir():
  return BUILD_DIR

def getQtDir():
  with open(buildDir() + "/dllinfo.txt") as f:
    path = f.read().replace("/lib/cmake/Qt5", "")
    return path

def ship(file):
  print("Copying", file)
  if os.path.isdir(file):
    shutil.copytree(file, "release/" + os.path.basename(file))
  else:
    shutil.copy(file, "release")
  
def createInstaller():
  print("Starting installer creation")
  
  binary_creator = shutil.which("binarycreator")
  
  if binary_creator is None:
    qt_if_path = os.environ.get("QT_IF_DIR")
    if qt_if_path is None:
      print("Could not find binarycreator, skipping installer creation")
      return None
    binary_creator = qt_if_path + "/bin/binarycreator.exe"
  
  if os.path.exists(buildDir() + "/installer"):
    shutil.rmtree(buildDir() + "/installer")
  
  shutil.copytree("installer", buildDir() + "/installer")
  shutil.copytree("release", buildDir() + "/installer/packages/github.bandicode.ffmpeggui/data")
  
  subprocess.run([binary_creator, "--offline-only", "-c", "config/config.xml", "-p", "packages", "installer.exe"], cwd=buildDir() + "/installer")
  shutil.move(buildDir() + "/installer/installer.exe", buildDir() + "/installer.exe")
  

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

  for p in QT_PLUGINS:
    ship(qt_path + "/plugins/" + p)

  for f in glob.glob("release/**/*.pdb"):
    print("Removing", f)
    os.remove(f)
  for f in glob.glob("release/**/*d.dll"):
    print("Removing", f)
    os.remove(f)
	
  createInstaller()
  
if __name__ == "__main__":
  main()
