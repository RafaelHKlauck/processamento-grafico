import cv2
import numpy as np
from rules import can_active_filter

active_filters = []
cap = None

def get_active_filters():
  return active_filters

def clear_filters():
  global active_filters
  active_filters = []

def blurFilter(image):
  # GaussianBlur é um filtro de suavização que reduz o ruído da imagem 
  return cv2.GaussianBlur(image, (13, 13), 5, 0)

def grayFilter(image):
  # Converter a imagem para tons de cinza
  return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

def hsvFilter(image):
  # Converter a imagem para o espaço de cor HSV (Hue, Saturation, Value)
  return cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

def brightnessFilter(image):
  # Aumentar o brilho da imagem
  return cv2.convertScaleAbs(image, alpha=1.5, beta=40)

def cannyFilter(image):
  # Detectar bordas na imagem
  return cv2.Canny(image, 100, 200)

def invertFilter(image):
  # Inverter as cores da imagem (negativo)
  return cv2.bitwise_not(image)

def pencilSketchFilter(image):
  # Aplicar um efeito de desenho a lápis na imagem
  # sigma_s é o desvio padrão do filtro gaussiano aplicado à imagem
  # sigma_r é o desvio padrão do filtro gaussiano aplicado à diferença de intensidade
  # shade_factor é um fator que controla a intensidade do efeito de sombreamento
  gray, sketch = cv2.pencilSketch(image, sigma_s=60, sigma_r=0.07, shade_factor=0.05)
  return sketch

def summerFilter(image):
  # Converter a imagem para o espaço de cor HSV
  hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
  h, s, v = cv2.split(hsv)

  # Aumentar a saturação
  s = cv2.add(s, 50)
  s = np.clip(s, 0, 255)

  # Aumentar o brilho
  v = cv2.add(v, 30)
  v = np.clip(v, 0, 255)

  # Recombinar os canais e converter de volta para BGR 
  final_hsv = cv2.merge((h, s, v))
  summer_image = cv2.cvtColor(final_hsv, cv2.COLOR_HSV2BGR)
  return summer_image

def winterFilter(image):
  # Converter a imagem para o espaço de cor HSV
  hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
  h, s, v = cv2.split(hsv)

  # Reduzir a saturação para dar uma aparência mais fria
  s = cv2.add(s, -50)
  s = np.clip(s, 0, 255)

  # Ajustar os tons de azul aumentando o valor nos tons mais escuros
  v = cv2.add(v, -30)
  v = np.clip(v, 0, 255)

  # Recombinar os canais e converter de volta para BGR
  final_hsv = cv2.merge((h, s, v))
  winter_image = cv2.cvtColor(final_hsv, cv2.COLOR_HSV2BGR)

  # Adicionar uma leve tonalidade azulada para enfatizar o efeito de inverno
  blue_channel = winter_image[:, :, 0]
  blue_channel = cv2.add(blue_channel, 50)
  blue_channel = np.clip(blue_channel, 0, 255)
  winter_image[:, :, 0] = blue_channel

  return winter_image

def sepiaFilter(image):
  # Fotografias antigas
  # Esse filtro aplica uma transformação de cor para dar uma aparência sépia(tom marrom-avermelhado) à imagem
                        # R     G     B 
  kernel = np.array([[0.272, 0.534, 0.131],
                       [0.349, 0.686, 0.168],
                       [0.393, 0.769, 0.189]])
  # Aplicar a transformação sépia
  sepia_image = cv2.transform(image, kernel)
  sepia_image = np.clip(sepia_image, 0, 255)
  return sepia_image.astype(np.uint8)

filters = {
  'blur': blurFilter,
  'hsv': hsvFilter,
  'gray': grayFilter,
  'brightness': brightnessFilter,
  'canny': cannyFilter,
  'invert': invertFilter,
  'pencilSketch':pencilSketchFilter,
  'summer': summerFilter,
  'winter': winterFilter,
  'sepia': sepiaFilter
}

def apply_filters(img):
  global active_filters
  filtered_img = img.copy()
  for filter_name in active_filters:
    filter_function = filters.get(filter_name)
    if filter_function:
      filtered_img = filter_function(filtered_img)
  
  return filtered_img

def handleFilter(_, filter):
  global active_filters
  if not can_active_filter(active_filters, filter):
    return
  if filter in active_filters:
    active_filters.remove(filter)
  else:
    active_filters.append(filter)
