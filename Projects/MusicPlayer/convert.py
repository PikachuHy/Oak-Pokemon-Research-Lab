from PIL import Image

def transPNG(src, dst):
    img = Image.open(src)
    img = img.convert('RGBA')
    datas = img.getdata()
    new_data = []
    for item in datas:
        if item[0] > 225 and item[1] > 225 and item[2] > 225:
            new_data.append((255, 255, 255, 0))
        else:
            new_data.append(item)

    img.putdata(new_data)
    img.save(dst, "PNG")

transPNG("disk_600x600.jpeg", "disk_600x600.png")