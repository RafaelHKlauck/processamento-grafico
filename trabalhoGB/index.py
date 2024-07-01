import cv2
from tkinter import Tk, filedialog, Button
from filterHandler import *
from stickerHandler import *


def create_buttons():
  # Filters
  cv2.createButton('Blur', handleFilter, 'blur', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('HSV', handleFilter, 'hsv', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Gray', handleFilter, 'gray', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Brightness', handleFilter, 'brightness', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Canny', handleFilter, 'canny', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Invert', handleFilter, 'invert', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Pencil Sketch', handleFilter, 'pencilSketch', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Summer', handleFilter, 'summer', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Winter', handleFilter, 'winter', cv2.QT_PUSH_BUTTON, 1)
  cv2.createButton('Sepia', handleFilter, 'sepia', cv2.QT_PUSH_BUTTON, 1)

  # Stickers
  cv2.createButton('None', handleSticker, '', cv2.QT_RADIOBOX, 0)
  cv2.createButton('Cry', handleSticker, 'cry', cv2.QT_RADIOBOX, 0)
  cv2.createButton('Heart', handleSticker, 'heart', cv2.QT_RADIOBOX, 0)
  cv2.createButton('Laugh', handleSticker, 'laugh', cv2.QT_RADIOBOX, 0)
  cv2.createButton('Sad', handleSticker, 'sad', cv2.QT_RADIOBOX, 0)
  cv2.createButton('Smile', handleSticker, 'smile', cv2.QT_RADIOBOX, 0)

def show_webcam_content():
  global cap

  while True:
    ret, image = cap.read()
    if not ret:
      print("Erro ao capturar frame.")
      break
    
    image_with_stickers = apply_stickers(image)
    final_image = apply_filters(image_with_stickers)
    active_filters = get_active_filters()
    cv2.putText(final_image, 'Active filters: ' + ', '.join(active_filters), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1, cv2.LINE_AA)
    cv2.imshow("webcam", final_image)

    # Aguardar a entrada do teclado para sair (27 é ESC) ou 'q' 
    k = cv2.waitKey(1) & 0xFF
    if k == 27 or k == 113:
      break
  
  cv2.destroyAllWindows()
  if cap:
    cap.release()


def open_webcam():
  global cap
  clear_filters()
  clear_stickers()

  num = input("Pressione 0 para abrir a webcam. E 1 para escolher uma webcam externa: ")
  if num == '1':
    cap = cv2.VideoCapture(1)
  else:
    cap = cv2.VideoCapture(0) 
  if not cap.isOpened():
    print("Erro ao abrir a webcam.")
    return
  cv2.namedWindow("webcam")
  cv2.setMouseCallback("webcam", handle_click)
  create_buttons()
  show_webcam_content()


def open_image():
  clear_filters()
  clear_stickers()
  filename = filedialog.askopenfilename(title="Abrir Imagem")
  if filename:
    img = cv2.imread(filename)
    cv2.namedWindow("Imagem Carregada")
    cv2.setMouseCallback("Imagem Carregada", handle_click)
    create_buttons()

    if img is not None:
      while True:
        img_copy = img.copy()
        img_with_sticker = apply_stickers(img_copy)
        final_image = apply_filters(img_with_sticker)
        active_filters = get_active_filters()
        cv2.putText(final_image, 'Active filters: ' + ', '.join(active_filters), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1, cv2.LINE_AA)
        cv2.imshow("Imagem Carregada", final_image)

        # Aguardar a entrada do teclado para sair (27 é ESC) ou 'q'
        k = cv2.waitKey(1) & 0xFF
        if k == 27 or k == 113:
          break

      cv2.destroyAllWindows()

def main():
  root = Tk()
  root.title("Seleção de Filtros")
  
  Button(root, text="Abrir Imagem", command=open_image).pack(padx=20, pady=20)
  Button(root, text="Iniciar Webcam", command=open_webcam).pack(padx=20, pady=20)
  Button(root, text="Fechar", command=root.quit).pack(padx=20, pady=20)

  root.mainloop()

if __name__ == "__main__":
  main()
