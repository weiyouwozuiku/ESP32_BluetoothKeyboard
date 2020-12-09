import time
import subprocess
from board import SCL, SDA
import busio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306
import RPi.GPIO as GPIO
import os
Touch = 12
GPIO.setmode(GPIO.BCM)
GPIO.setup(Touch, GPIO.IN)
# Create the I2C interface.
i2c = busio.I2C(SCL, SDA)

disp = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

disp.fill(0)
disp.show()

width = disp.width
height = disp.height
image = Image.new("1", (width, height))

draw = ImageDraw.Draw(image)

draw.rectangle((0, 0, width, height), outline=0, fill=0)

padding = -2
top = padding
bottom = height - padding
# Move left to right keeping track of the current x position for drawing shapes.
x = 0

font = ImageFont.truetype(
    '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 13)

def displayInfo():
    draw.rectangle((0, 0, width, height), outline=0, fill=0)

    cmd = "hostname -I | cut -d' ' -f1"
    IP = subprocess.check_output(cmd, shell=True).decode("utf-8")
    cmd = "top -bn1 | grep load | awk '{printf \"CPU Load: %.2f\", $(NF-2)}'"
    CPU = subprocess.check_output(cmd, shell=True).decode("utf-8")
    cmd = "free -m | awk 'NR==2{printf \"Mem:%s/%s %.2f%%\", $3,$2,$3*100/$2 }'"
    MemUsage = subprocess.check_output(cmd, shell=True).decode("utf-8")
    cmd = 'df -h | awk \'$NF=="/"{printf "Disk:%d/%d GB  %s", $3,$2,$5}\''
    Disk = subprocess.check_output(cmd, shell=True).decode("utf-8")

    # Write four lines of text.

    draw.text((x, top + 0), "IP: " + IP, font=font, fill=255)
    draw.text((x, top + 15), CPU, font=font, fill=255)
    draw.text((x, top + 31), MemUsage, font=font, fill=255)
    draw.text((x, top + 47), Disk, font=font, fill=255)

    # Display image.
    disp.image(image)
    disp.show()
    time.sleep(0.1)
    
def logToFile(filePath, *words) -> bool:
    with open(filePath, 'a+', encoding="utf8")as f:
        for word in words:
            print(word, file=f, end="")
        print(file=f)
        return True
    return False

def checkTouch():
    info = GPIO.input(Touch)
    # print(info)
    if info == 1:
        logToFile(filePath, time.strftime(
            "%Y-%m-%d %H:%M:%S", time.localtime()))
        logToFile(filePath, "*"*30)
        logToFile(filePath, "FingerPrint Start!")
        os.system("sudo /home/pi/hub-ctrl.c/hub-ctrl -b 001 -d 002 -P 2 -p 1")
        draw.rectangle((0, 0, width, height), outline=0, fill=0)
        draw.text((x+10, top + 16), "FingerPrint Start!", font=font, fill=255)
        disp.image(image)
        disp.show()
        time.sleep(60)
        logToFile(filePath, "FingerPrint Stop!\n"+"*"*30+"\n")
        os.system("sudo /home/pi/hub-ctrl.c/hub-ctrl -b 001 -d 002 -P 2 -p 0")

pwd = os.getcwd()
filePath = os.path.join(pwd, "FingerLog.txt")
while True:
    displayInfo()
    checkTouch()
