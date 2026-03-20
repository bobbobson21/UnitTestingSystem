const { app, dialog, BrowserWindow, ipcMain } = require("electron")
const { exec } = require("child_process");
const { debug } = require("console");

const path = require("path");
const fs = require("fs");
const { debuglog } = require("util");

function LoadBridges(window)
{
// create file /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle('CreateFile', (req, data) => {
        if(data === undefined || data.title === undefined || data.type === undefined || data.contents === undefined) {return {success: false};}
        
        var filePath = "";

        if(data.fromRootPath == null)
        {
            filePath = __dirname;
        }
        else
        {
            filePath = data.fromRootPath;
        }

        if(data.addedPath != null)
        {
            filePath = filePath + "\\" + data.addedPath;
        }

        var filePath = filePath + "\\" + data.title + "." + data.type;
        var completedWithNoErrors = true;

        fs.writeFileSync(filePath, data.contents, (err)=>{
            if(err)
            {
                completedWithNoErrors = false;
            }
        });

        return {success: completedWithNoErrors, path: filePath, contents: data.content };
    });

// read file //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle('ReadFile', (req, data) => {
        if(data === undefined || data.title === undefined || data.type === undefined) {return {success: false};}
        
        var filePath = "";

        if(data.fromRootPath == null)
        {
            filePath = __dirname;
        }
        else
        {
            filePath = data.fromRootPath;
        }

        if(data.addedPath != null)
        {
            filePath = filePath + "\\" + addedPath;
        }

        var filePath = filePath + "\\" + data.title + "." + data.type;
        var completedWithNoErrors = true;

        const fileContent = fs.readFileSync(filePath, {encoding:"utf8", flag: "r"}, (err, data) => {
            if(err)
            {
                completedWithNoErrors = false;
            }
        });

        return {success: completedWithNoErrors, path: filePath, contents: fileContent };
    });

// mod window /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle( 'ModWindow', (req, data) => {
        if(data.min == true)
        {
            window.minimize();
        }

        if(data.max == true)
        {
            if(window.isMaximized() == false)
            {
                window.maximize();
            }
            else
            {
                window.unmaximize();
            }
        }

        if(data.close == true)
        {
            window.close();
        }
    });

// select folder /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle( 'SelectDirectoryAsset', (req, data) => {
        const openResults = dialog.showOpenDialogSync(window, {title: data.title, properties: data.properties});

        if(openResults != null)
        {
            return {success: true, results: openResults}
        }

        return {success: false};
    });
}

function createWindow()
{
    const win = new BrowserWindow({
        width: 1200,
        height: 800,
        minWidth: 900,
        minHeight: 600,
        //frame: false, //commnet this to bring back old window
        //autoHideMenuBar: true, //commnet this to bring back old window
        webPreferences:{
            preload: path.join(__dirname, "preload.js")
        }
    });
    

    LoadBridges(win);

    win.setBackgroundColor("#000000");
    win.loadFile("scr/index.html");

}

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
    if(process.platform !== 'darwin')
    {
        app.quit();
    }
})