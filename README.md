[![Language](https://img.shields.io/badge/Language-English-blue)](#english)
[![Язык](https://img.shields.io/badge/Язык-Русский-red)](#russian)

[![Download windows](https://img.shields.io/badge/Download-Windows-brightgreen)](https://github.com/Divetoxx/Mandelbrot/releases/latest/download/Mandelbrot_windows_x64.zip)
[![Download linux](https://img.shields.io/badge/Download-Linux-orange?logo=linux&logoColor=white)](https://github.com/Divetoxx/Mandelbrot/releases/latest/download/Mandelbrot_linux_x64.tar.gz)
[![Source Code](https://img.shields.io/badge/Source_Code-ZIP-orange?logo=github)](https://github.com/Divetoxx/Mandelbrot/archive/refs/heads/main.zip)
[![Source Code](https://img.shields.io/badge/Source_Code-TAR.GZ-lightgrey?logo=github)](https://github.com/Divetoxx/Mandelbrot/archive/refs/heads/main.tar.gz)


<a name="english"></a>
# 🇺🇸 English Version

# Mandelbrot CLI: Renderer with Perturbation Theory (with a $10^{-308}$ hardware limit)

## Credits & Acknowledgments

This project implements advanced orbit phase management paradigms and perturbation algorithms 
developed by the fractal research community. Special thanks to the following authors and pioneers 
from the Fractal Forums, whose collaborative work made this engine possible:
* **Kevin Martin** - for pioneering loop optimization and edge-case escape techniques.
* **Zhuoran Yu** - for developing the dynamic orbit re-basing paradigm.
* **Claude Heiland-Allen** - for extensive deep zoom research and the creation of the MDZ project.

## Key Features:

* **High-Precision Reference:** The 5000-bit reference trajectory is computed exactly once per zoom layer.
* **Hardware-Native Performance:** Blazing-fast math for billions of pixels utilizing hardware-native double registers.
* When using double-precision floating-point numbers (on the order of $10^{-15}$), perturbation theory only allows you to zoom down to the **$10^{-308}$ level-no further.**
* **Innovative Algorithm:** Revolutionary *Reference Reset to Zero* implementation.
* **True 8x8 SSAA:** Pristine, anti-aliased image quality with 64 independent samples per pixel.
* **OpenMP Multi-threading:** High-speed parallel computing to maximize CPU utilization.

## Arbitrary Precision Arithmetic (Infinite Depth)

The engine is completely free from the hardware limitations of standard 64-bit (`double`) and 128-bit (`__float128`) data types, 
which inevitably lose significance and produce pixelated blocks at depths beyond $10^{-15}$ and $10^{-34}$ respectively.
* **MPFR/GMP Integration**: All high-precision navigation, mouse clicking, and keyboard arrow zooming are handled within a deep binary memory 
structure configured to **5000-bit precision**.
* **308-Digit Decimal:** Viewport coordinates are stored in `Mandelbrot.txt`. While the MPFR 
reference radar computes the center at 5000-bit precision, the hardware native `double` exponent limits the fast pixel engine to a maximum 
depth of $10^{-308}$, providing pristine clarity up to this physical silicon barrier.


## Blazing Fast Perturbation Theory

Deep fractal rendering no longer requires heavy "long-division-style" arbitrary precision math for every individual pixel, which 
historically slowed down deep zoom software by thousands of times.
* **Single-Pass Reference Calculation**: The ultra-heavy MPFR BigFloat radar calculates the precise reference trajectory 
for **only one central pixel per frame and strictly ONCE** at the beginning of the render.
* **Hardware-Accelerated Double Math**: The rest of the viewport (billions of super-sampled pixels) is processed concurrently at the 
native speed of the CPU's hardware `double` registers, calculating only tiny deviations (deltas) from the central axis. 
This optimization boosts rendering speeds by up to 1000x!

## Revolutionary Reference Reset to Zero Algorithm

This is a tremendous point of pride: the engine now operates under the exact same mathematical and architectural principles as the world's most advanced.
* **Dynamic Reset to Zero**: Now, each pixel checks the ratio of its full coordinates against 
its current delta at every step. If the delta grows too large or the central reference orbit terminates, the thread 
dynamically **resets its reading index to zero**, turning the accumulated coordinates into a new autonomous origin.
* **One-Step Beyond Escape Loop Optimization**: To squeeze maximum performance out of the CPU, the MPFR reference radar records 
exactly **one additional point** into the reference orbit array immediately after it exceeds the escape radius.
* **Elimination of Branching (Branch Unrolling)**: This elegant trick completely eliminated cumbersome `if` and `OR` conditions 
from the deepest iteration loop. The processor no longer wastes clock cycles on branch prediction, allowing the compiler 
to perfectly vectorize the math.

## Project Purpose

This application is a high-performance command-line (CLI) utility designed for rendering high-fidelity frame sequences of the Mandelbrot set. 
It focuses on precision and offline rendering rather than real-time navigation.

## Key Features:

*    Animation Ready (Frame Sequences): The utility automatically generates a sequence of 255 BMP frames with a smooth palette rotation 
effect. These frames are ready to be encoded into high-quality video (e.g., via FFmpeg).
*    Extreme SSAA 8x8: Implements a massive 8x8 Super Sampling Anti-Aliasing. Each final pixel is derived from 64 independent 
sub-samples, resulting in unparalleled image clarity and zero aliasing noise.
*    CLI Automation: Choose a preset location (1-6) or load custom coordinates from Mandelbrot.txt, and let the engine 
handle the heavy math and file I/O.

## One-Shot Variety: 

*    The utility generates 255 different color variations for your chosen location in a single run. Instead of re-rendering to find the perfect 
look, you can simply browse the output folder and pick the most aesthetically pleasing frame. Using the Palette Shifting method, 
the heavy fractal math is calculated only once, while all 255 variations are produced almost instantly.

## Life Hack: "Live" Animation in File Explorer

You can experience the Color Rotation effect without using a video player!
*    Open the folder containing the rendered frames (Mandelbrot000.bmp to Mandelbrot254.bmp).
*    Open the first image in the default Windows Photo Viewer.
*    Hold down the Right Arrow key on your keyboard or quickly spin your mouse wheel.
*    Since the program has generated all 255 color variations, the fractal will "come to life" right before your eyes.

## Optional: Rendering a Video

If you want to see how these colors flow, you can compile all 255 frames into a video (30 FPS) using FFmpeg.
You can download the pre-compiled FFmpeg binary from my repository:

**[Download FFmpeg windows](https://github.com/Divetoxx/Mandelbrot-Video/releases/latest/download/Mandelbrot_windows_x64.zip)**

**[Download FFmpeg linux](https://github.com/Divetoxx/Mandelbrot-Video/releases/latest/download/Mandelbrot_linux_x64.tar.gz)**

Use the following command to encode the frames into a Mandelbrot.mp4 file: 

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v libx264 -refs 6 -me_method umh -partitions all -psy 0 -qp 18 -subq 9 -me_range 24 -deblock -6:-6 -bf 6 -i_qfactor 2 -trellis 0 -b_strategy 2 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

If you have an NVIDIA graphics card, you can significantly speed up the encoding process:

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v h264_nvenc -b:v 50M -profile:v high -coder 1 -rc-lookahead 32 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

## OpenMP

OpenMP is a standard that tells the compiler, "Take this loop and distribute the iterations among the different processor cores."
Yes, using OpenMP you are doing parallel programming at the Multithreading level.
Everything is powered by **OpenMP** parallel loops for maximum performance.
OpenMP - Scalability: Your code will run equally efficiently on a 4-core laptop and a 128-core server.

## 8x8 Supersampling (64 Samples Per Pixel)

Super-Sampling Anti-Aliasing (SSAA) is a high-end technique increasing samples per pixel to enhance image quality, 
with 8x (N=8) rendering scenes at 8x resolution on both axes to produce 64 samples per pixel. 

I decided to take the visual quality to a completely different level. This engine implements
True 8x8 Supersampling Anti-Aliasing (SSAA) with 64 independent samples per single screen pixel, utilizing Direct RGB-Space Integration.
After calculating all 64 samples for a pixel, they are downsampled into one.
Key Technical Advantages:
*    64-Point Fractal Sampling: Each final screen pixel is computed from sixty-four independent fractal coordinate points.
*    High-Precision Per-Channel RGB Accumulation: The engine first calculates the specific 24-bit color for every single sub-pixel before performing any blending.
*    Noise Elimination: By accumulating color intensities (R, G, B) rather than raw iteration counts, we completely eliminate "chromatic noise." The result is a crystal-clear, razor-sharp image where every micro-filament is perfectly reconstructed.
*    True Color Integration: Our solution performs integration directly in the RGB color space. By computing the exact Red, Green, and Blue 
components for each sub-pixel before downsampling, we achieve a cinematic level of smoothness and structural integrity 
that 8-bit or iteration-based renderers simply cannot match.

## Generating 255 Frames: Optimization Strategy

This is an efficient pre-render strategy: we calculate the heavy mathematics (iteration counts) 
once, store the raw data, and then rapidly generate frames by shifting colors and downsampling.
Since calculating a fractal 255 times is computationally expensive, we split the task into two stages.

Stage 1: Iteration Map Generation (Raw Data)

Instead of BMP files, we create a single data buffer where we store only the iteration number (t) for each pixel.

Stage 2: 255-Frame Rendering (Color + Anti-aliasing)

We read the iteration map and perform the following for each frame:
*    Downsample: Process an 8x8 pixel block from the high-res map.
*    Color Mapping: Map each pixel value to a shifted color palette.
*    Smoothing: Average the colors (Supersampling Anti-Aliasing) to produce a final frame.

## Visual Aesthetics

The Red, Green, and Blue channels are calculated using sine and cosine waves to create smooth color transitions:
```C++
        pal[a][0] = (uint8_t)round(127.0 + 127.0 * cos(2.0 * PI * a / 255.0)); // Blue
        pal[a][1] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Green
        pal[a][2] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Red
```

## The Mandelbrot Set: A Mathematical Absolute

It is truly one of the few objects that connects us to something absolutely objective and infinite, 
transcending biology and history. Even if our entire universe and all its atoms were to vanish tomorrow, 
the equation would remain true. It is not "written" on the stars; it is embedded in the structure of logic itself. 
This makes the Mandelbrot Set a kind of absolute.
 
The Mandelbrot set exists independently of our minds and technology. It is an infinite mathematical structure that 
has always existed.

## Controls & Preset Selection (CLI)

Since this is a command-line interface (CLI) application, navigation is handled by entering the location number at program startup.

| Action | Input | Description |
| :--- | :--- | :--- |
| **Presets** | `1` - `6` + `ENTER` | Instantly select one of the 6 unique deep-zoom locations. |
| **Custom Point** | `7` + `ENTER` | Load coordinates (`absc`, `ordi`, `size`) from `Mandelbrot.txt`. |
| **Exit** | `Any other` + `ENTER` | Safely close the application. |


```C++
case 1: absc_str = "-1.7491976289657893741942376816272921165326158557416159"; 
ordi_str = "-0.00000042530777152440422725855012159249401150956515248"; 
size_str = "4.31e-51"; break;
case 2: absc_str = "-1.74907816150520173167912454515663360420734509948112463480292338384"; 
ordi_str = "-0.00000550991906629096602513098567268615714673236269915508056068145"; 
size_str = "1.53e-62"; break;
case 3: absc_str = "-1.7489436617686633372073552153211507258063533373824414679761"; 
ordi_str = "-0.0000073748967541889836640985849393311615399776865199722998"; 
size_str = "1.01e-55"; break;
case 4: absc_str = "-1.7491311840575335110236048528001036247123430447933925298694915282522178938437759580179"; 
ordi_str = "-0.0001996960702541036804654299663680287246637758588467627907752429037173153157138373325"; 
size_str = "2.84e-82"; break;
case 5: absc_str = "-1.74994586497557459407526067070055710001"; 
ordi_str = "-0.0000000852088539604644334731909824511"; 
size_str = "7.17e-36"; break;
case 6: absc_str = "-1.267078059171397835210199054200436920994876769284288837862647"; 
ordi_str = "-0.123788215196292957558264285607075473360968832625384429809391"; 
size_str = "2.4e-57"; break;
```

## Mandelbrot.txt File Structure

To load custom coordinates (option 7 in the menu), create a Mandelbrot.txt file in the application folder. 
The file must contain three numbers separated by a newline:
*    Abscissa (Center X coordinate)
*    Ordinate (Center Y coordinate)
*    Size (Zoom level/Area size)

Example file content:

![Mandelbrot txt](Mandelbrot.png)

## Look at the results! The smoothness is incredible 

![Mandelbrot Set](Mandelbrot%20Set%20Image%20A.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20B.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20C.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20D.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20E.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20F.jpg)

**[Download Latest Version (Windows & Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**



<a name="russian"></a>
# 🇷🇺 Русская версия

# Консольный рендерер Мандельброта с методом возмущений и предела $10^{-308}$.

## Благодарности (Credits)

Этот проект использует передовые математические алгоритмы и идеи динамического управления фазой орбит, 
разработанные фрактальным сообществом. Особая благодарность авторам и исследователям с Fractal Forums, 
чей совместный труд лег в основу этого движка:
* **Kevin Martin** - автор фундаментальных методов векторизации и оптимизации циклов возмущений.
* **Zhuoran Yu** - разработчик концепции динамического сброса орбит.
* **Claude Heiland-Allen** - исследователь экстремального фрактального приближения и создатель проекта MDZ.


## Ключевые особенности:

* Расчёт опорной траектории на 5000 бит всего один раз.
* Реактивный расчёт миллиарда пикселей на аппаратном double.
* При использовании чисел с плавающей запятой двойной точности (порядка $10^{-15}$) теория возмущений позволяет приблизиться к уровню **$10^{-308}$ - не дальше.**
* Революционный алгоритм Reference Reset to Zero.
* Настоящий SSAA 8x8 для идеально сглаженного изображения без алиасинга.
* Параллелизм OpenMP для высокоскоростного многопоточного рендеринга.

## Безграничная точность (Arbitrary Precision Arithmetic)

Движок полностью избавлен от аппаратных ограничений 64-битных (`double`) и 128-битных (`__float128`) чисел, которые неизбежно слепнут и выдают 
пиксельные квадраты на глубинах более $10^{-15}$ и $10^{-34}$.
* **Интеграция MPFR/GMP**: Вся высокоточная навигация, пересчёт масштаба при кликах мыши и движении стрелочками клавиатуры ведутся 
внутри сверхглубокой бинарной памяти с точностью **5000 бит**!
* **308 десятичных знаков**: 
Координаты кадра сохраняются и считываются из файла `Mandelbrot.txt`. Навигация и радар MPFR работают на глубине до 5000 бит, 
однако скоростной пиксельный дельта-движок ограничен аппаратной экспонентой `double`, что позволяет исследовать безупречно четкие 
структуры на запредельных масштабах **вплоть до $10^{-308}$ знаков**.


## Реактивный метод возмущений (Perturbation Theory)

Рендеринг глубоких фракталов больше не требует тяжелых вычислений <в столбик> для каждого пикселя, что обычно замедляло программы в тысячи раз.
* **Однократный расчёт опоры**: Сверхтяжелый BigFloat-радар MPFR вычисляет точную траекторию всего для **одной-единственной центральной точки 
кадра и строго ОДИН раз** в начале рендеринга.
* **Аппаратное ускорение на double**: Весь остальной массив экрана (миллиарда супер-пикселей) рассчитывается параллельно на бешеной скорости 
чистых, аппаратных регистров `double` процессора, вычисляя лишь микроскопические отклонения (дельты) от центральной оси. 
Скорость генерации взлетела в 1000 раз!

## Революционный алгоритм Reference Reset to Zero

Это огромный повод для гордости. Ваша программа теперь работает по тем же математическим принципам, что и самые передовые фрактальные движки в мире.
* **Динамический сброс на ноль**: Теперь пиксель на каждом шаге проверяет соотношение своих полных координат и дельты. Если дельта становится 
слишком большой или кэш центра иссякает, поток прямо на лету **сбрасывает индекс чтения на ноль**, превращая накопленные координаты 
в новую автономную точку.
* **Хакерская оптимизация цикла (One-Step Beyond Escape)**: Чтобы выжать максимум скорости из процессора, радар MPFR записывает 
строго **одну дополнительную точку** в массив опорной орбиты сразу после того, как она превышает радиус ухода. 
* **Уничтожение ветвлений (Branch Unrolling)**: Этот изящный трюк позволил полностью избавиться от громоздких `if` и `OR`-условий 
внутри самого глубокого цикла итераций. Процессор больше не тратит такты на предсказание переходов, а компилятор смог идеально векторизовать 
код.

## О проекте

Данное приложение представляет собой консольную утилиту (CLI) для высокопроизводительного рендеринга 
последовательностей кадров множества Мандельброта. В отличие от интерактивных визуализаторов, эта программа 
ориентирована на создание высококачественных заготовок для видео и сверхчётких изображений.

## Что она делает:

*    Генерация анимации (Frame Sequences): Программа автоматически создает 255 последовательных кадров (.bmp) с 
эффектом ротации палитры. Эти кадры можно легко объединить в плавное видео (например, через FFmpeg).
*    Экстремальный Суперсэмплинг (SSAA 8x8): Программа использует колоссальный уровень сглаживания. Каждый пиксель 
финального изображения вычисляется на основе 64 независимых выборок. Это обеспечивает идеальную чистоту картинки даже 
в самых зашумленных зонах фрактала.
*    Batch Processing: Работает полностью в автоматическом режиме через командную строку. Вы выбираете точку (1-6), и 
программа выполняет всю тяжелую работу по расчету и сохранению файлов.

## Мгновенная вариативность: 

*    Программа генерирует 255 различных вариантов раскраски для выбранной локации за один проход. Вам не нужно запускать рендер 
снова и снова, чтобы подобрать идеальный вид - просто откройте папку и выберите самый красивый кадр из готовой серии. 
Благодаря методу Palette Shifting, расчет математики происходит один раз, а все 255 изображений создаются практически мгновенно.

## Лайфхак: <Живая> анимация в проводнике

Вы можете увидеть эффект Color Rotation без видеоплеера!
*    Откройте папку с готовыми кадрами (Mandelbrot000.bmp - Mandelbrot254.bmp).
*    Откройте первое изображение во встроенном просмотре Windows.
*    Просто зажмите стрелку Вправо на клавиатуре или быстро крутите колесико мыши.
*    Благодаря тому, что программа создала все 255 вариантов, фрактал <оживет> прямо у вас на глазах.

## Дополнительно: Рендеринг видео

Если вы хотите увидеть, как эти цвета перетекают, вы можете скомпилировать все 255 кадров в видео (30 кадров в секунду) с помощью FFmpeg.
Вы можете скачать предварительно скомпилированный бинарный файл FFmpeg из моего репозитория:

**[Скачать FFmpeg виндовз](https://github.com/Divetoxx/Mandelbrot-Video/releases/latest/download/Mandelbrot_windows_x64.zip)**

**[Скачать FFmpeg линукс](https://github.com/Divetoxx/Mandelbrot-Video/releases/latest/download/Mandelbrot_linux_x64.tar.gz)**

Используйте следующую команду для кодирования кадров в файл Mandelbrot.mp4: 

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v libx264 -refs 6 -me_method umh -partitions all -psy 0 -qp 18 -subq 9 -me_range 24 -deblock -6:-6 -bf 6 -i_qfactor 2 -trellis 0 -b_strategy 2 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

Если у вас видеокарта NVIDIA, вы можете значительно ускорить процесс кодирования:

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v h264_nvenc -b:v 50M -profile:v high -coder 1 -rc-lookahead 32 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

## OpenMP

OpenMP - это стандарт, который говорит компилятору: "Возьми этот цикл и сам раздай итерации разным ядрам процессора".
Используя OpenMP, вы занимаетесь параллельным программированием на уровне многопоточности (Multithreading).
OpenMP - масштабируемость: ваш код будет одинаково эффективно работать как на 4-ядерном ноутбуке,
так и на 128-ядерном сервере.

## Суперсэмплинг 8x8 (64 прохода на один пиксель)

Суперсэмплинг (SSAA) - ресурсоемкий метод сглаживания, увеличивающий число выборок на пиксель для повышения качества изображения. 
При значении 8x (N=8) сцена рендерится в разрешении, в 8 раз превышающем целевое, по обеим осям, создавая 64 (или 8 х 8) выборки 
на пиксель. Изображение просчитывается в более высоком разрешении, а затем принудительно уменьшается до разрешения дисплея, 
устраняя лесенки и улучшая чёткость.

Я решил вывести качество изображения на совершенно новый уровень. Этот движок использует
истинное сглаживание 8x8 Supersampling Anti-Aliasing (SSAA) с 64 независимыми сэмплами на каждый пиксель экрана, используя прямую интеграцию в RGB-пространство.

После вычисления всех 64 сэмплов для пикселя, они уменьшаются до одного.
Ключевые технические преимущества:
*  64-точечное фрактальное сэмплирование: каждый конечный пиксель экрана вычисляется из шестидесяти четырех независимых 
фрактальных координатных точек.
*  Высокоточное накопление RGB-цвета по каналам: движок сначала вычисляет конкретный 24-битный цвет для каждого субпикселя, 
прежде чем выполнять какое-либо смешивание.
*  Устранение шума: Накапливая интенсивность цвета (R, G, B), а не просто подсчитывая количество итераций, мы полностью 
устраняем <хроматический шум>. В результате получается кристально чистое, резкое изображение, где каждая микронить идеально воссоздана.
*  Интеграция истинного цвета: Наше решение выполняет интеграцию непосредственно в цветовом пространстве RGB. 
Вычисляя точные компоненты красного, зеленого и синего цветов для каждого субпикселя перед понижением разрешения, 
мы достигаем кинематографического уровня плавности и структурной целостности, недостижимого для 8-битных или итерационных рендеров.

## Генерация 255 кадров

Это отличная стратегия оптимизации! Вы хотите применить пререндер: сначала рассчитать тяжелую математику 
(номера итераций) один раз, сохранить их, а затем быстро генерировать кадры, просто меняя цвета и уменьшая размер.
Поскольку считать 255 раз - это безумие, мы разделим задачу на два этапа.

Этап 1: Генерация <карты итераций> (Raw Data)

Вместо BMP мы создадим один огромный файл, где для каждого пикселя запишем только число t (номер итерации). 

Этап 2: Генерация 255 кадров (Цвет + Сглаживание)

Теперь мы читаем эту карту и для каждого кадра делаем:
Берем блок 8x8 пикселей из большой карты.
Красим каждый пиксель согласно сдвинутой палитре.
Усредняем цвета (это и есть сглаживание) и записываем в файл.
 
## Визуальная эстетика

Красный, зеленый и синий каналы рассчитываются с использованием синусоидальных и косинусоидальных волн для создания плавных цветовых переходов:
```C++
        pal[a][0] = (uint8_t)round(127.0 + 127.0 * cos(2.0 * PI * a / 255.0)); // Blue
        pal[a][1] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Green
        pal[a][2] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Red
```

## Множество Мандельброта: Математический абсолют

Это поистине один из немногих объектов, который связывает нас с чем-то абсолютно объективным и бесконечным,
превосходящим биологию и историю. Даже если бы вся наша Вселенная и все её атомы исчезли завтра,
уравнение осталось бы верным. Оно не <написано> на звёздах; оно заложено в самой структуре логики.
Это делает множество Мандельброта своего рода абсолютом.
 
Множество Мандельброта существует независимо от нашего разума и технологий. 
Это бесконечная математическая структура, которая существовала всегда.

## Управление и выбор локаций (CLI Controls)

Поскольку это консольное приложение, управление осуществляется через ввод номера локации при запуске программы.

| Действие | Ввод | Описание |
| :--- | :--- | :--- |
| **Пресеты** | `1` - `6` + `ENTER` | Выбор одной из 6 встроенных точек мандельброта - глубокого зума. |
| **Своя точка** | `7` + `ENTER` | Загрузка координат (`absc`, `ordi`, `size`) из файла `Mandelbrot.txt`. |
| **Выход** | `Любая клавиша` + `ENTER` | Завершение работы программы. |


```C++
case 1: absc_str = "-1.7491976289657893741942376816272921165326158557416159"; 
ordi_str = "-0.00000042530777152440422725855012159249401150956515248"; 
size_str = "4.31e-51"; break;
case 2: absc_str = "-1.74907816150520173167912454515663360420734509948112463480292338384"; 
ordi_str = "-0.00000550991906629096602513098567268615714673236269915508056068145"; 
size_str = "1.53e-62"; break;
case 3: absc_str = "-1.7489436617686633372073552153211507258063533373824414679761"; 
ordi_str = "-0.0000073748967541889836640985849393311615399776865199722998"; 
size_str = "1.01e-55"; break;
case 4: absc_str = "-1.7491311840575335110236048528001036247123430447933925298694915282522178938437759580179"; 
ordi_str = "-0.0001996960702541036804654299663680287246637758588467627907752429037173153157138373325"; 
size_str = "2.84e-82"; break;
case 5: absc_str = "-1.74994586497557459407526067070055710001"; 
ordi_str = "-0.0000000852088539604644334731909824511"; 
size_str = "7.17e-36"; break;
case 6: absc_str = "-1.267078059171397835210199054200436920994876769284288837862647"; 
ordi_str = "-0.123788215196292957558264285607075473360968832625384429809391"; 
size_str = "2.4e-57"; break;
```

## Структура файла Mandelbrot.txt

Для загрузки пользовательских координат (пункт 7 в меню), создайте текстовый файл Mandelbrot.txt в папке с программой. 
Файл должен содержать три числа, разделенных переносом строки:
*    Abscissa (Координата X центра)
*    Ordinate (Координата Y центра)
*    Size (Масштаб/Размер области)

Пример содержания файла:

![Mandelbrot txt](Mandelbrot.png)

## Посмотрите на результаты! Невероятная плавность работы

![Mandelbrot Set](Mandelbrot%20Set%20Image%20A.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20B.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20C.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20D.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20E.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%20F.jpg)

**[Скачать последнюю версию (Windows и Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**

