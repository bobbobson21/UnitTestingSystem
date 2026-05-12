@echo off

echo this process requires access to the whole npm system which may cause a volitile reaction up on first exacution if the terminal closes and it isnt by your hand restart it.

pause

IF NOT EXIST %cd%\node_modules\.bin\electron.cmd (
npm install -D
)

npm start
pause