#Step 1 - Capture video from video source
#step 2 - Detect the hand
#Step 3 - SEparate points
#step 4 - Use the mouse pointe using our finger
#Step 5 -Click option

import cv2
import mediapipe as mp
import pyautogui

def landmark_coordinates(hand, landmark_id, frame_height, frame_width):
    landmark = hand.landmark[landmark_id]
    return int(landmark.x *frame_width ), int (landmark.y *frame_height)
def start_video():
    screen_width, screen_height = pyautogui.size()
    capture = cv2.VideoCapture(0)
    hand_detector = mp.solutions.hands.Hands()
    draw = mp.solutions.drawing_utils

    hand_color = mp.solutions.drawing_utils.DrawingSpec(color=(0, 255, 0), thickness= 3, circle_radius=2)
    hand_connection = mp.solutions.drawing_utils.DrawingSpec(color=(0, 255, 0), thickness=3 )

    while True:
        check, frame = capture.read()
        frame = cv2.flip(frame, 1)
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = hand_detector.process(rgb_frame)
        hands = result.multi_hand_landmarks

        if hands:
            for hand in hands:
                draw.draw_landmarks(frame, hand, mp.solutions.hands.HAND_CONNECTIONS,
                                    hand_color,
                                    hand_connection)

                frame_height , frame_width,_ = frame.shape
                tx, ty = landmark_coordinates(hand,4 ,frame_height, frame_width)
                x ,y = landmark_coordinates(hand,8, frame_height, frame_width)
                #print(x,y)

                cv2.circle(img=frame, center=(tx, ty), radius=11, color=(255,0,0), thickness=10)
                tx_pos = screen_width/frame_width *tx
                ty_pos = screen_height/frame_height *ty
                cv2.circle(img=frame, center= (x,y), radius= 11, color=(0,255,255), thickness= 10)
                x_pos = screen_width/frame_width * x
                y_pos = screen_height/frame_height * y
                y_axis_distance = abs(y_pos - ty_pos)
                print(y_axis_distance)
                if y_axis_distance < 80:
                    print("click")
                    pyautogui.click()
                    pyautogui.sleep(2)
                pyautogui.moveTo(x_pos,y_pos)
        if not check:
            break
        cv2.imshow("Mouse Project", frame)
        if cv2.waitKey(1) == ord('c'):
            break

    capture.release()
    cv2.destroyAllWindows()

start_video()
