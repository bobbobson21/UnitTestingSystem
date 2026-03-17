const { app, dialog, BrowserWindow, ipcMain } = require("electron")
const { exec } = require("child_process");
const { debug } = require("console");

const path = require("path");
const fs = require("fs");

function LoadBridges(window)
{
// create file /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle('CreateFile', (req, data) => {
        if(!data || !data.title || !data.type || !data.contents) {return {success: false};}
        
        var filePath = __dirname;

        if(data.path != null)
        {
            filePath = data.path;
        }

        if(data.addedPath != null)
        {
            filePath  = filePath + "/" + addedPath;
        }

        filePath = filePath + "/" + data.title + "." + data.type;
        var completedWithNoErrors = true;

        fs.writeFileSync(filePath, data.contents, (err)=>{
            if(err)
            {
                console.error("failed to write to file at: " + filePath);
                completedWithNoErrors = false;
            }
            else
            {
                console.log("file write successfully at:" + filePath);
            }
        });

        return {success: completedWithNoErrors, path: filePath, contents: data.content };
    });

// read file //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle('ReadFile', (req, data) => {
        if(!data || !data.title || !data.type) {return {success: false};}
        
        var filePath = __dirname;

        if(data.path != null)
        {
            filePath = data.path;
        }

        if(data.addedPath != null)
        {
            filePath  = filePath + "/" + addedPath;
        }

        filePath = filePath + "/" + data.title + "." + data.type;
        let completedWithNoErrors = true;

        const fileContent = fs.readFileSync(filePath, {encoding:"utf8", flag: "r"}, (err, data) => {
            if(err)
            {
                console.error("failed to read file at: " + filePath);
                 completedWithNoErrors = false;
            }
            else
            {
                console.log("file read successfully at:" + filePath);
            }
        });

        return {success: completedWithNoErrors, path: filePath, contents: fileContent };
    });

// delete file (stub) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ipcMain.handle('DeleteFile', (req, data) => {
        if(!data || !data.title || !data.type) {return {success: false};}
        var filePath = __dirname;

        if(data.path != null)
        {
            filePath = data.path;
        }

        if(data.addedPath != null)
        {
            filePath = filePath + "/" + addedPath;
        }

        filePath = filePath + "/" + data.title + "." + data.type;
        var completedWithNoErrors = true;

        fs.unlinkSync(filePath, (err) => {
            if(err)
            {
                console.error("failed to delete file at: " + filePath);
                 completedWithNoErrors = false;
            }
            else
            {
                console.log("file deleted successfully at:" + filePath);
            }
        })

        return {success: completedWithNoErrors};
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