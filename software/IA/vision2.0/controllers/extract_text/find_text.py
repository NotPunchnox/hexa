import pytesseract

def find_text(cv2, frame):
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
    
    text = pytesseract.image_to_string(frame)
    cv2.putText(frame, f"text détécté: {text}", (240, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

    print("txt:", text)
    return frame