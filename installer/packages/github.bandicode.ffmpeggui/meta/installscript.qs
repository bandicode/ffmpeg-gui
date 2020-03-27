
function Component()
{

}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
    } catch (e) {
        console.log(e);
    }
    
    // Create shortcut 
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/ffmpeg-gui.exe", "@StartMenuDir@/ffmpeg-gui.lnk",
            "workingDirectory=@TargetDir@", "description=Launch ffmpeg GUI");
    }
}
