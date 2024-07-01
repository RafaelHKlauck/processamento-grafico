import cv2
import os
import numpy as np
from Sticker import Sticker

base_sticker_path = 'assests/'
stickers = {
  'cry': 'cry.png',
  'heart': 'heart.png',
  'laugh': 'laugh.png',
  'sad': 'sad.png',
  'smile': 'smile.png'
}

stickers_scale = {
  'cry': 10,
  'heart': 30,
  'laugh': 10,
  'sad': 20,
  'smile': 15
}
active_stickers = []
current_sticker = None

def clear_stickers():
  global active_stickers, current_sticker
  active_stickers = []
  current_sticker = None

def overlay(background, sticker, x_offset=None, y_offset=None):
  bg_h, bg_w, bg_channels = background.shape
  fg_h, fg_w, fg_channels = sticker.shape

  assert bg_channels == 3, f'background image should have exactly 3 channels (RGB). found:{bg_channels}'
  assert fg_channels == 4, f'sticker image should have exactly 4 channels (RGBA). found:{fg_channels}'

  # center by default
  if x_offset is None: x_offset = (bg_w - fg_w) // 2
  if y_offset is None: y_offset = (bg_h - fg_h) // 2

  w = min(fg_w, bg_w, fg_w + x_offset, bg_w - x_offset)
  h = min(fg_h, bg_h, fg_h + y_offset, bg_h - y_offset)

  if w < 1 or h < 1: return

  # clip sticker and background images to the overlapping regions
  bg_x = max(0, x_offset)
  bg_y = max(0, y_offset)
  fg_x = max(0, x_offset * -1)
  fg_y = max(0, y_offset * -1)
  sticker = sticker[fg_y:fg_y + h, fg_x:fg_x + w]
  background_subsection = background[bg_y:bg_y + h, bg_x:bg_x + w]

  # separate alpha and color channels from the sticker image
  sticker_colors = sticker[:, :, :3]
  alpha_channel = sticker[:, :, 3] / 255  # 0-255 => 0.0-1.0

  # construct an alpha_mask that matches the image shape
  alpha_mask = np.dstack((alpha_channel, alpha_channel, alpha_channel))

  # combine the background with the sticker image weighted by alpha
  composite = background_subsection * (1 - alpha_mask) + sticker_colors * alpha_mask

  # overwrite the section of the background image that has been updated
  background[bg_y:bg_y + h, bg_x:bg_x + w] = composite

  return background

def handle_click(event, x, y, _, __):
  global current_sticker
  if event == cv2.EVENT_LBUTTONDOWN and current_sticker:
    sticker_image_path = os.path.join(base_sticker_path, stickers[current_sticker])
    sticker_image = cv2.imread(sticker_image_path, cv2.IMREAD_UNCHANGED)
    if sticker_image is not None:
      # Calcula a largura e altura do sticker redimensionado
      width = int(sticker_image.shape[1] * stickers_scale[current_sticker] / 100)
      height = int(sticker_image.shape[0] * stickers_scale[current_sticker] / 100)
      
      # Ajusta as coordenadas para centralizar o sticker corretamente
      x -= width // 2
      y -= height // 2
      
      # Cria o objeto Sticker com as coordenadas ajustadas
      sticker = Sticker(x, y, stickers_scale[current_sticker], sticker_image)
      active_stickers.append(sticker)

def handleSticker(_, sticker):
  global current_sticker
  current_sticker = sticker

def apply_stickers(original_img):
  img = original_img.copy()
  
  for sticker in active_stickers:
    x, y = sticker.posX, sticker.posY
    sticker_img = sticker.image
    
    # Chama a função overlay para aplicar o sticker na imagem original
    img = overlay(img, sticker_img, x, y)
  
  return img
