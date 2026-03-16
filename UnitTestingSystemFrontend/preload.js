const {contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld( "api", {    
    minYear: new Date().getFullYear(), //how far back they can go to pervent issues
    maxYear: new Date().getFullYear() +20, //how far they can go
    year: new Date().getFullYear(), //current year
    month: new Date().getMonth() +1, //current month
    monthDay: new Date().getDate(), //the current day of the month
    weekDay: new Date().getDay(), //the current week day

    CreateFile: (data) => ipcRenderer.invoke("CreateFile", data),
    ReadFile: (data) => ipcRenderer.invoke("ReadFile", data),
    ModWindow: (data) => ipcRenderer.invoke("ModWindow",data)
});