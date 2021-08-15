#!/bin/ipython

import argparse
import numpy as np
import matplotlib.pyplot as plt
import sys

## general defines

linecolor = "#%x%x%x" % ( 217, 234, 211 )
markercolor = "#%x%x%x" % ( 217/2, 234/2, 211/2 )

# Draw pretty plot
def doc_plot(fig, x, y):

  plt.figure(fig.number)
  fig.clear()

  lines, = plt.plot(x,y)
  lines.set_color(linecolor)
  lines.set_linewidth(4)
  lines.set_marker('o')
  lines.set_markeredgecolor(markercolor)
  lines.set_markersize(6)
  lines.set_markeredgewidth(2)

  axes = fig.get_axes()[0]
  axes.set_aspect(1)
  axes.set_ybound(0,1)
  axes.set_xbound(0,1)
  axes.grid(True)
  axes.xaxis.label.set_text(r'$P_{IN}$')
  axes.xaxis.label.set_fontsize(14)
  axes.yaxis.label.set_text(r'$P_{OUT}$')
  axes.yaxis.label.set_fontsize(14)

# Print out interleaved coefficients for HAL3 tonemap curve tags
def doc_coeff(x,y):
  coeffs = np.vstack((x, y)).reshape(-1,order='F')
  coeff_str = "[ "
  for val in coeffs[:-1]:
    coeff_str += "%0.4f, " % val

  coeff_str += "%0.4f ]" % coeffs[-1]

  print coeff_str

def doc_map(fig, imgMap, index):
  plt.figure(fig.number)
  fig.clear()
  plt.imshow(imgMap - 1, interpolation='nearest')
  for x in range(0, np.size(imgMap, 1)):
    for y in range(0, np.size(imgMap, 0)):
      plt.text(x,y, imgMap[y,x,index], color='white')

  axes = fig.get_axes()[0]
  axes.set_xticks(range(0, np.size(imgMap, 1)))
  axes.set_yticks(range(0, np.size(imgMap, 0)))

## Check arguments

parser = argparse.ArgumentParser(description='Draw plots for camera HAL3.x implementation spec doc')
parser.add_argument('--save_figures', default=False, action='store_true',
                   help='Save figures as pngs')

args = parser.parse_args()

## Linear mapping

x_lin = np.linspace(0,1,2)
y_lin = x_lin

lin_fig = plt.figure(1)
doc_plot(lin_fig, x_lin, y_lin)

lin_title = 'Linear tonemapping curve'
plt.title(lin_title)
print lin_title
doc_coeff(x_lin, y_lin)

if args.save_figures:
  plt.savefig('linear_tonemap.png',bbox_inches='tight')

## Inverse mapping

x_inv = x_lin
y_inv = 1 - x_lin

inv_fig = plt.figure(2)
doc_plot(inv_fig, x_inv, y_inv)

inv_title = 'Inverting tonemapping curve'
plt.title(inv_title)
print inv_title
doc_coeff(x_inv, y_inv)

if args.save_figures:
  plt.savefig('inverse_tonemap.png',bbox_inches='tight')

## Gamma 1/2.2

x_gamma = np.linspace(0, 1, 16);

y_gamma = x_gamma**(1/2.2)

gamma_fig = plt.figure(3)
doc_plot(gamma_fig, x_gamma, y_gamma)

gamma_title = r'$\gamma=1/2.2$ tonemapping curve'
plt.title(gamma_title)
print gamma_title
doc_coeff(x_gamma, y_gamma)

if args.save_figures:
  plt.savefig('gamma_tonemap.png',bbox_inches='tight')

## sRGB curve

x_srgb = x_gamma
y_srgb = np.where(x_srgb <= 0.0031308, x_srgb * 12.92, 1.055*x_srgb**(1/2.4)-0.055)

srgb_fig = plt.figure(4)
doc_plot(srgb_fig, x_srgb, y_srgb)

srgb_title = 'sRGB tonemapping curve'
plt.title(srgb_title)
print srgb_title
doc_coeff(x_srgb, y_srgb)

if args.save_figures:
  plt.savefig('srgb_tonemap.png',bbox_inches='tight')

## Sample lens shading map

shadingMapSize = np.array([3, 4])
shadingMap1 = np.array(
    [ 1.3, 1.2, 1.15, 1.2,  1.2, 1.2, 1.15, 1.2,  1.1, 1.2, 1.2, 1.2,  1.3, 1.2, 1.3, 1.3,
      1.2, 1.2, 1.25, 1.1,  1.1, 1.1, 1.1, 1.0,   1.0, 1.0, 1.0, 1.0,  1.2, 1.3, 1.25, 1.2,
      1.3, 1.2, 1.2, 1.3,   1.2, 1.15, 1.1, 1.2,  1.2, 1.1, 1.0, 1.2,  1.3, 1.15, 1.2, 1.3 ])
redMap = shadingMap1[0::4].reshape(shadingMapSize)
greenEMap = shadingMap1[1::4].reshape(shadingMapSize)
greenOMap = shadingMap1[2::4].reshape(shadingMapSize)
blueMap = shadingMap1[3::4].reshape(shadingMapSize)

rgbMap = np.dstack( (redMap, (greenEMap + greenOMap) / 2, blueMap) )
redMap = np.dstack( (redMap, np.zeros(shadingMapSize), np.zeros(shadingMapSize) ) )
greenEMap = np.dstack( (np.zeros(shadingMapSize), greenEMap, np.zeros(shadingMapSize) ) )
greenOMap = np.dstack( (np.zeros(shadingMapSize), greenOMap, np.zeros(shadingMapSize) ) )
blueMap = np.dstack( (np.zeros(shadingMapSize), np.zeros(shadingMapSize), blueMap ) )

redImg = plt.figure(5)
doc_map(redImg, redMap, 0)
plt.title('Red lens shading map')

if args.save_figures:
  plt.savefig('red_shading.png',bbox_inches='tight')

greenEImg = plt.figure(6)
doc_map(greenEImg, greenEMap, 1)
plt.title('Green (even rows) lens shading map')

if args.save_figures:
  plt.savefig('green_e_shading.png',bbox_inches='tight')

greenOImg = plt.figure(7)
doc_map(greenOImg, greenOMap, 1)
plt.title('Green (odd rows) lens shading map')

if args.save_figures:
  plt.savefig('green_o_shading.png',bbox_inches='tight')

blueImg = plt.figure(8)
doc_map(blueImg, blueMap, 2)
plt.title('Blue lens shading map')

if args.save_figures:
  plt.savefig('blue_shading.png',bbox_inches='tight')

rgbImg = plt.figure(9)

rgbImg.clear()
plt.imshow(1/rgbMap,interpolation='bicubic')

axes = rgbImg.get_axes()[0]
axes.set_xticks(range(0, np.size(rgbMap, 1)))
axes.set_yticks(range(0, np.size(rgbMap, 0)))

plt.title('Image of uniform white wall (inverse shading map)')

if args.save_figures:
  plt.savefig('inv_shading.png',bbox_inches='tight')

# Rec. 709
x_rec709 = x_gamma
y_rec709 = np.where(x_rec709 <= 0.018, x_rec709 * 4.500, 1.099*x_rec709**0.45-0.099)

rec709_fig = plt.figure(10)
doc_plot(rec709_fig, x_rec709, y_rec709)

rec709_title = 'Rec. 709 tonemapping curve'
plt.title(rec709_title)
print rec709_title
doc_coeff(x_rec709, y_rec709)

if args.save_figures:
  plt.savefig('rec709_tonemap.png',bbox_inches='tight')


# Show figures

plt.show()
