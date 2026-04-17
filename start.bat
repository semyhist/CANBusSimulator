@echo off
echo Derleniyor...
"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" -Wall -o bin/can_sim.exe src/main.c src/can_frame.c src/can_bus.c src/can_node.c src/ecu_motor.c src/ecu_abs.c src/ecu_dashboard.c src/ecu_airbag.c src/ecu_klima.c src/ecu_direksiyon.c src/ecu_kabin.c src/ecu_lastik.c src/can_json.c src/can_logger.c src/can_parser.c src/can_fuzzer.c src/can_detector.c

if %errorlevel% neq 0 (
    echo Derleme hatasi!
    pause
    exit /b 1
)

echo Simulator baslatiliyor...
start "" bin\can_sim.exe

echo Dashboard baslatiliyor...
cd dashboard
start "" cmd /k "npm run dev"
cd ..

echo.
echo Simulator : calisiyor
echo Dashboard : http://localhost:5173
