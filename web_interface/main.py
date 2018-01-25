import base64
import enum
import io
from typing import List
from flask import Flask, request, render_template, send_file
import serial

ser = serial.Serial('/dev/ttyUSB0')  # open serial port
app = Flask(__name__)  # create the application instance :)

try:
    import picamera

    global camera
    camera = picamera.PiCamera()
    camera.resolution = (1024, 768)
    # camera.start_preview()

except:
    pass


class PacketType(enum.Enum):
    MOVE = "move"
    SOUND = "sound"

    @staticmethod
    def from_string(input: str):
        if input == "move":
            return PacketType.MOVE
        return PacketType.SOUND


class Movement(enum.Enum):
    FORWARD = "forward"
    BACKWARD = "backward"
    LEFT = "left"
    RIGHT = "right"

    @staticmethod
    def from_string(input: str):
        return {
            "forward": Movement.FORWARD,
            "backward": Movement.BACKWARD,
            "left": Movement.LEFT,
            "right": Movement.RIGHT,
        }[input]


class Song(enum.Enum):
    SONG_1 = "sound1"

    @staticmethod
    def from_string(input: str):
        return {
            "sound1": Song.SONG_1
        }[input]


def to_packet_duration_bytes(duration: int) -> List[int]:

    mask = 0x7F
    b = [(duration >> 7) & mask, duration & mask]
    for a in b:
        print("duration: " + str(b))
    return b

print((to_packet_duration_bytes(2000)[0] << 7) | to_packet_duration_bytes(2000)[1])


def send_packet_bytes(bytes_to_send: List[int]):
    temp = [
        0xFF
    ]

    for byte in bytes_to_send:
        temp.append(int(byte))
    temp.append(0xFE)

    ser.write(bytes(temp))


def send_packet_move(diraction: Movement, duration: int):
    packet_digit = 0x00
    diractions = {
        Movement.FORWARD: 31,
        Movement.BACKWARD: 95,
        Movement.LEFT: 63,
        Movement.RIGHT: 0,
    }
    packet_diraction = diractions[diraction]
    packet_duration = to_packet_duration_bytes(duration)
    packet_bytes = [
        packet_digit,
        packet_diraction,
        packet_duration[0],
        packet_duration[1]
    ]

    send_packet_bytes(packet_bytes)


def send_packet_sound(song: Song, duration: int):
    packet_digit = 0x01
    songs = {
        Song.SONG_1: 0
    }

    packet_song = songs[song]
    packet_duration = to_packet_duration_bytes(duration)
    packet_bytes = [
        packet_digit,
        packet_song,
        packet_duration[0],
        packet_duration[1]
    ]
    send_packet_bytes(packet_bytes)


def send_packet(packet_type: PacketType, sub_action: str, duration: int):
    if packet_type == PacketType.MOVE:
        send_packet_move(Movement.from_string(sub_action), duration)
    elif packet_type == PacketType.SOUND:
        send_packet_sound(Song.from_string(sub_action), duration)


@app.route('/', methods=['GET', 'POST'])
def route_home():
    if request.method == 'POST':
        action = request.form["action"]
        sub_action = request.form["sub-action"]
        duration = request.form["duration"]
        send_packet(PacketType.from_string(action), sub_action, int(duration))

    return render_template("home.html", data="test", data2="asasdasd")


@app.route('/image')
def route_image():
    my_stream = io.BytesIO()
    camera.capture(my_stream, 'jpeg', use_video_port=True)
    image_64_encode = base64.b64encode(my_stream.getvalue())

    return "<img src=\"data:image/png;base64," + image_64_encode.decode(
            "UTF-8") + "\"/><style>*{margin:0;padding:0}</style>"


app.run(host='0.0.0.0')
ser.close()
