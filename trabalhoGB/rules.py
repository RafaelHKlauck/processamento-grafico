
def can_active_filter(active_filters, filter):
  # Equalize
  if (filter == 'hsv'):
    return 'gray' not in active_filters and 'canny' not in active_filters
  if (filter == 'gray'):
    return 'equalize' not in active_filters and 'canny' not in active_filters
  if (filter == 'pencilSketch'):
    return 'gray' not in active_filters and 'canny' not in active_filters
  if (filter == 'sepia'):
    return 'gray' not in active_filters and 'canny' not in active_filters
  if (filter == 'summer'): 
    return 'gray' not in active_filters and 'canny' not in active_filters
  if (filter == 'winter'):
    return 'gray' not in active_filters and 'canny' not in active_filters
  return True