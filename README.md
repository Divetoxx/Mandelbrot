# Mandelbrot set fragment.

The palette generation is done here. 0 is Green, 1 is Blue, and 2 is Red.

pal[a][0]:=round(127+127cos(2pia/255));

pal[a][1]:=round(127+127sin(2pia/255));

pal[a][2]:=Random(256)

And I decided-this would be a completely different level of quality! I implemented true supersampling (antialiasing)-with 8x8 antialiasing (64 passes per pixel!!!)
That is, not 1920 by 1080 pixels, but 8x8 more! 15360 by 8640 pixels! And then these 64 passes are smoothly reduced by one pixel-and no longer 8-bit, but 24-bit TrueColor!
Creates an executable file and creates Mandelbrot.bmp
OpenMP - Scalability: Your code will run equally efficiently on a 4-core laptop and a 128-core server without any program modifications.


Генерация палитры сделана вот. 0 - это Green, 1 - это Blue и 2 - это Red.

pal[a][0]:=round(127+127*cos(2*pi*a/255));

pal[a][1]:=round(127+127*sin(2*pi*a/255));

pal[a][2]:=Random(256)

Но где же сами изображения? Я хочу увидеть Множество Мандельброта!
И вот тут начинается самое интересное. Если вы посмотрите на большинство программ, вы увидите проблему: всего 256 цветов и явные <ступеньки> между цветовыми переходами (так называемый цветовой бандинг).
Я не смог на это смотреть без боли. А что же делают другие разработчики? Я изучил популярные проекты:

https://mathr.co.uk/kf/kf.html

https://www.juliasets.dk/Mandelbrot.htm

https://math.hws.edu/eck/js/mandelbrot/java/MandelbrotSettings/

https://www.ultrafractal.com/

https://xaos-project.github.io/

Четыре из пяти проектов вообще статичны! Только пятый, Xaos, предлагает анимацию. Но все они ограничены палитрой в 256 цветов.
Мне это не нравится! И я решил - будет совершенно другой уровень качества! Я реализовал честный суперсэмплинг (антиалиасинг) - со сглаживанием 8x8 (64 прохода на один пиксель!!!)
То есть не 1080 на 1080 пикселя а в 8x8 больше! 8640 на 8640 пикселя! А потом эти 64 прохода уменьшают на один пиксель но плавно - и уже не 8-битного а 24-битного цвета TrueColor!
И тоже параллельный цикл OpenMP.
Но самое главное - КАРТИНКИ ))) Смотрите:

<p align="center">
![Изображение фрактала Мандельброта]
(Mandelbrot.bmp)</p>


**[Скачать последнюю версию тут](https://github.com/Divetoxx/Mandelbrot/releases)**






