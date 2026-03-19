const {contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld( "api", {    
    //CreateFile: (data) => ipcRenderer.invoke("CreateFile", data),
    ReadFile: (data) => ipcRenderer.invoke("ReadFile", data),
    ModWindow: (data) => ipcRenderer.invoke("ModWindow",data),
    SelectDirectoryAsset: (data) => ipcRenderer.invoke("SelectDirectoryAsset",data)
});