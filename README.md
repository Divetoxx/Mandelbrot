[![Language](https://img.shields.io/badge/Language-English-blue)](#english)
[![Язык](https://img.shields.io/badge/Язык-Русский-red)](#russian)

[![Download windows](https://img.shields.io/badge/Download-Windows-brightgreen)](https://github.com/Divetoxx/Mandelbrot/releases/latest/download/Mandelbrot_windows_x64.zip)
[![Download linux](https://img.shields.io/badge/Download-Linux-orange?logo=linux&logoColor=white)](https://github.com/Divetoxx/Mandelbrot/releases/latest/download/Mandelbrot_linux_x64.tar.gz)
[![Source Code](https://img.shields.io/badge/Source_Code-ZIP-orange?logo=github)](https://github.com/Divetoxx/Mandelbrot/archive/refs/heads/main.zip)
[![Source Code](https://img.shields.io/badge/Source_Code-TAR.GZ-lightgrey?logo=github)](https://github.com/Divetoxx/Mandelbrot/archive/refs/heads/main.tar.gz)


<a name="english"></a>
# 🇺🇸 English Version

# Mandelbrot CLI: Renderer with Perturbation Theory and Bilinear approximation


## Credits & Acknowledgments

This project implements advanced orbit phase management paradigms and perturbation algorithms developed 
by the fractal research community. Special thanks to the following authors and pioneers from the Fractal Forums, 
whose collaborative work made this engine possible:

* **Kevin Martin** — for pioneering loop optimization and edge-case escape techniques.
* **Zhuoran Yu** — for developing the dynamic reference relocalization and orbit re-basing paradigm.
* **James Ward Martin (JWM)** — for the creation of the pioneering MDZ (Mandelbrot Deep Zoom) project utilizing MPFR/GMP libraries.
* **Claude Heiland-Allen** — for extensive deep zoom mathematical research, preservation of fractal software legacy, 
and advanced palette conversion algorithms.


## Key Features:

* **High-Precision Reference:** The 1000-bit reference trajectory is computed exactly once per zoom layer using GNU MPFR.
* **Hardware-Native Performance:** Blazing-fast math for millions of pixels utilizing hardware-native double registers.
* When using double-precision floating-point numbers (on the order of $10^{-15}$), perturbation theory only allows you 
to zoom down to the **$10^{-308}$ level—no further.**
* **Dynamic Reference Relocalization** (Orbit Re-basing).
* **Bilinear approximation (BLA Skip-100):** The calculation can be performed significantly faster by skipping 100 iterations 
at once on smooth areas.
* **True 8x8 SSAA (Direct RGB Integration):** Pristine, anti-aliased image quality with 64 independent samples per pixel 
blended directly in RGB color space to eliminate chromatic noise.
* **Pure C++11 Multi-threading:** High-speed parallel computing built directly into the language standard, scaling 
transparently across CPU architectures.



## Arbitrary Precision Arithmetic (Infinite Depth)

The engine is completely free from the hardware limitations of standard data types—such as the native 64-bit (`double`) 
and the extended 80-bit (`long double`) floats—which inevitably lose significance and produce pixelated 
blocks at precision depths beyond $10^{-15}$ and $10^{-19}$ respectively.

* **MPFR/GMP Integration:** All high-precision coordinate navigation, mouse-click viewport targeting, and grid step 
calculations are handled within an isolated binary memory structure configured to a pristine **1000-bit precision**.
* **308-Digit Decimal Silicon Barrier:** The dynamic exponent range of the hardware-native `double` register limits 
the high-speed delta pixel engine to a maximum theoretical depth of **$10^{-308}$ digits**. Current viewport coordinates 
are safely stored and retrieved via the `Mandelbrot.txt` configuration file, guaranteeing flawless structural clarity 
of mathematical micro-filaments right up to this physical silicon boundary.



## High-Speed Perturbation Theory

Rendering extreme close-ups no longer requires arbitrary-precision arithmetic for each individual pixel.

* **High-Precision Reference Orbit:** MPFR BigFloat computes the exact trajectory for **one central pixel once** per frame.
* **Hardware-Accelerated Computation:** The rest of the screen is processed via hardware `double` registers by 
calculating relative deviations (deltas), accelerating frame generation up to 1000x.




## Dynamic Reference Relocalization (Orbit Re-basing)

This is a tremendous point of pride: the engine operates under the exact same mathematical and architectural principles 
as the world's most advanced Mandelbrot renderers.
*   **Dynamic Reset to Zero**: At every step, each pixel monitors the ratio of its absolute coordinates against its current delta. 
If the delta drifts too far or the primary reference orbit terminates, the execution thread dynamically 
**resets its tracking index to zero**, establishing the accumulated coordinates as a new autonomous origin 
and eliminating precision artifacts.
*   **One-Step Beyond Escape Loop Optimization**: To squeeze maximum performance out of the CPU, the MPFR reference 
tracker records exactly **one additional point** into the orbit array immediately after exceeding the escape radius. 
This precise boundary definition guarantees flawless orbit re-basing.
*   **Deep Loop Branch Optimization**: This elegant layout significantly streamlines conditions within the deepest 
iteration loop. By minimizing heavy branching and avoiding complex nested conditions where possible, the code improves 
branch prediction efficiency and allows the CPU pipeline to execute the core mathematics with maximum throughput.


## Bilinear Approximation

The rendering engine achieves a massive performance leap by integrating the Bilinear Approximation 
method (**BLA Skip-100**).

* **Coefficient Matrix Precomputation:** The algorithm precomputes the transformation matrices for intervals of exactly 
100 iterations.
* **Massive Iteration Skipping:** If the current deviation (delta) point lies safely within the computed radius 
of convergence relative to the reference orbit, the execution thread skips 100 iterations at once in a single mathematical 
step. This drastically minimizes CPU clock cycles and delivers an exponential speedup across the smooth, 
uniform filaments of the Mandelbrot set.


## Pure C++11 Multi-threading

High-speed parallel computing built directly into the language standard. Scales transparently across architectures-delivering 
flawless 100% CPU utilization whether running on a 4-core laptop or a 128-core high-performance server without external dependencies.


## True 8x8 SSAA: 64-Sample Direct RGB Integration Space

To take visual quality to a completely unprecedented level, the engine implements **True 8x8 Super-Sampling Anti-Aliasing (SSAA)**, 
processing **64 independent sub-pixel fractal coordinates** per single final screen pixel to completely eliminate aliasing along 
the Mandelbrot set's complex boundaries.

### Mathematical Fallacy of the Standard Approach
Traditional rendering methods incorrectly average raw iteration counts or palette indices before applying color:
* **Standard Way:** `Color( (iter_1 + iter_2 + ... + iter_64) / 64 )`

Because fractal color palettes are strictly non-linear, calculating the arithmetic mean of iteration indices yields an 
unrelated color, resulting in jarring chromatic contours, pixelated artifacts, and high-contrast digital noise.

### Engineering Solution: 64-Point Optical RGB Integration
Instead of averaging raw algorithm steps, our engine computes the exact 24-bit/32-bit color for every single one 
of the 64 sub-pixels independently before any blending occurs. It then performs high-precision, per-channel accumulation 
of their color intensities:
* **Direct RGB Integration Algorithm:** `(Color(iter_1) + Color(iter_2) + ... + Color(iter_64)) / 64`

### Key Technical Advantages:
* **64-Point Fractal Sampling:** Every final screen pixel is reconstructed from sixty-four independent fractal coordinate 
points, capturing sub-pixel microstructures smaller than a physical monitor pixel.
* **Complete Noise Elimination:** By accumulating independent Red, Green, and Blue intensities prior to downsampling rather 
than raw iteration values, we completely eradicate "chromatic noise."
* **Cinematic Structural Integrity:** Performing integration directly in the RGB color space ensures a true optical blend. This delivers 
an ultra-smooth, continuous gradient and pristine image quality that standard 8-bit or index-based renderers simply cannot match.


## Generating 255 Frames: Optimization Strategy

This is an efficient pre-render strategy: we calculate the heavy mathematics (iteration counts) once, store the raw data, and then rapidly generate frames by shifting colors and downsampling.
Since calculating a 17280x17280 fractal 255 times is computationally expensive, we split the task into two stages.

### Stage 1: Iteration Map Generation (Raw Data)
Instead of BMP files, we create a single data buffer where we store only the iteration number (t) for each pixel.
* For 17280x17280 using `uint8_t`, the resulting file/buffer is approximately 298 MB.

### Stage 2: 255-Frame Rendering (Color + Anti-aliasing)
We read the iteration map and perform the following for each frame:
* **Downsample:** Process an 8x8 pixel block from the high-res map.
* **Color Mapping:** Map each pixel value to a shifted color palette.
* **Smoothing:** Average the colors (Supersampling Anti-Aliasing) to produce a final 2160x2160 frame.

### Why is this so fast?
* **Memory Efficiency:** The `iterMap` array (~298 MB) easily fits into modern RAM. The heavy perturbation calculation loop runs only once for the entire animation.
* **Palette Rotation:** Stage 2 completely avoids arbitrary-precision arithmetic and squaring. It only involves basic integer addition and lightning-fast memory lookups.
* **Parallelism:** Stage 2 is perfectly scalable. Thanks to our pure C++11 multi-threading engine, all frames are rendered simultaneously across all available CPU cores.
* **True Downsampling:** We implement honest 8x8 optical averaging, resulting in superior image quality compared to simple software resizing.

Once all 255 BMP files are generated, use FFmpeg to encode them into the final video.



## Visual Aesthetics

The Red, Green, and Blue channels are calculated using sine and cosine waves:
```C++
        pal[a][0] = (uint8_t)round(127.0 + 127.0 * cos(2.0 * PI * a / 255.0)); // Blue
        pal[a][1] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Green
        pal[a][2] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Red
```


## Project Purpose

This application is a high-performance command-line (CLI) utility designed for rendering high-fidelity frame sequences of the Mandelbrot set. 
It focuses on precision and offline rendering rather than real-time navigation.

## Key Features:

*    Animation Ready (Frame Sequences): The utility automatically generates a sequence of 255 BMP frames with a smooth palette rotation 
effect. These frames are ready to be encoded into high-quality video (e.g., via FFmpeg).
*    Extreme SSAA 8x8: Implements a massive 8x8 Super Sampling Anti-Aliasing. Each final pixel is derived from 64 independent 
sub-samples, resulting in unparalleled image clarity and zero aliasing noise.
*    CLI Automation: Choose a preset location (1-9) or load custom coordinates from Mandelbrot.txt, and let the engine 
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


## Controls & Preset Selection (CLI)

Since this is a command-line interface (CLI) application, navigation is handled by entering the location number at program startup.

| Action | Input | Description |
| :--- | :--- | :--- |
| **Presets** | `1` - `9` + `ENTER` | Instantly select one of the 9 unique deep-zoom locations. |
| **Custom Point** | `0` + `ENTER` | Load coordinates (`absc`, `ordi`, `size`) from `Mandelbrot.txt`. |
| **Rendering** | **Step 1 (Auto)** | High-precision calculation of 255 frames using **Multi-threading** and **SSAA 8x8**. |
| **Exit** | `Any other` + `ENTER` | Safely close the application. |


## Mandelbrot.txt File Structure

To load custom coordinates (option 0 in the menu), create a Mandelbrot.txt file in the application folder. 
The file must contain three numbers separated by a newline:
*    Abscissa (Center X coordinate)
*    Ordinate (Center Y coordinate)
*    Size (Zoom level/Area size)

Example file:
```C++
-1.758008286774384722
-0.012039512139649579
7.3e-15
```

## The Mandelbrot Set: A Mathematical Absolute

It is truly one of the few objects that connects us to something absolutely objective and infinite, 
transcending biology and history. Even if our entire universe and all its atoms were to vanish tomorrow, 
the equation would remain true. It is not "written" on the stars; it is embedded in the structure of logic itself. 
This makes the Mandelbrot Set a kind of absolute.
 
The Mandelbrot set exists independently of our minds and technology. It is an infinite mathematical structure that 
has always existed.


## Look at the results! The smoothness is incredible 

```C++
case 1: absc_str = "-1.74919762896578937419423768162729211653261585574161734756645570271259437106847945278504682550816934045282325615313706"; 
ordi_str = "-0.0000004253077715244042272585501215924940115095651514128365051648578746521975467051762910002043554114332125868777767"; 
size_str = "1.34e-113"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%201.jpg)

```C++
case 2: absc_str = "-1.74907816150520173167912454515663360420734509948112463480292338384"; 
ordi_str = "-0.00000550991906629096602513098567268615714673236269915508056068145"; 
size_str = "1.53e-62"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%202.jpg)

```C++
case 3: absc_str = "-1.7489436617686633372073552153211507258063533373824414679761"; 
ordi_str = "-0.0000073748967541889836640985849393311615399776865199722998"; 
size_str = "1.01e-55"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%203.jpg)

```C++
case 4: absc_str = "-1.7491311840575335110236048528001036247123430447933925298694915282522178938437759580179"; 
ordi_str = "-0.001996960702541036804654299663680287246637758588467627907752429037173153157138373325"; 
size_str = "2.84e-82"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%204.jpg)

```C++
case 5: absc_str = "-1.74994586497557459407526067070055710001"; 
ordi_str = "-0.0000000852088539604644334731909824511"; 
size_str = "7.17e-36"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%205.jpg)

```C++
case 6: absc_str = "-1.267078059171397835210199054200436920994876769284288837862647"; 
ordi_str = "-0.123788215196292957558264285607075473360968832625384429809391"; 
size_str = "2.4e-57"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%206.jpg)

```C++
case 7: absc_str = "-1.99999543561201124623198345433951143502785679245726844745821388800402678499411681518036306219179273434395557574279985918047221291197081186140687781560831995";
ordi_str = "-0.00000000000000000000000026198152173811047783694060060607013913873144250985383083459221663448338433592617272786772587281530484110756597337683912309313885172";
size_str = "1.15e-119"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%207.jpg)

```C++
case 8: absc_str = "-0.843233764530751513324611730003844788663891317894291476942";
ordi_str = "0.223818041558069959139898295480311440395574821870538878074";
size_str = "2.66e-52"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%208.jpg)

```C++
case 9: absc_str = "-0.12702288604303376058504940552835636145639339878096842408417373730142";
ordi_str = "-0.98728474138900681012772182210671973643707041031463043446236865072278";
size_str = "6.18e-64"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%209.jpg)



**[Download Latest Version (Windows & Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**



<a name="russian"></a>
# 🇷🇺 Русская версия

# Консольный рендерер Мандельброта с методом возмущений и билинейную аппроксимацию.

## Благодарности и признание авторства (Credits & Acknowledgments)

Этот проект использует передовые алгоритмы метода возмущений и концепции управления фазами орбит, 
разработанные мировым фрактальным сообществом. Особая благодарность следующим авторам и первопроходцам 
с Fractal Forums, чей совместный труд сделал создание этого движка возможным:

* **Kevin Martin** — за новаторские методы векторизации, оптимизацию глубоких циклов и точное определение границ ухода орбит.
* **Zhuoran Yu** — за разработку математической концепции динамической релокализации опорной точки и перебазирования орбит (Orbit Re-basing).
* **James Ward Martin (JWM)** — за создание фундаментального проекта MDZ (Mandelbrot Deep Zoom), заложившего основы применения 
библиотек MPFR/GMP для сверхглубокого погружения.
* **Claude Heiland-Allen** — за колоссальные математические исследования фрактального зума, сохранение исходного 
программного наследия сообщества и разработку продвинутых алгоритмов трансформации палитр.


## Ключевые особенности:

* **Высокоточная опорная орбита:** Опорная траектория с точностью 1000 бит вычисляется строго один раз для каждого слоя приближения 
с использованием библиотеки GNU MPFR.
* **Аппаратное ускорение:** Сверхбыстрые математические расчеты для миллионов пикселей за счет использования аппаратных регистров 
double процессора.
* При использовании чисел с плавающей запятой двойной точности (порядка $10^{-15}$) теория возмущений позволяет приблизиться 
к уровню **$10^{-308}$ — не дальше.**
* **Динамическая релокализация опорной точки** (Перебазирование орбиты / Orbit Re-basing).
* **Билинейная аппроксимация (BLA Skip-100):** Расчет выполняется существенно быстрее за счет одновременного «перепрыгивания» 
через 100 итераций на гладких участках фрактала.
* **Настоящий суперсэмплинг 8x8 SSAA (Прямая интеграция в RGB):** Безупречное, сглаженное качество изображения с 64 независимыми 
выборками на один пиксель. Смешивание происходит напрямую в цветовом пространстве RGB, что полностью устраняет хроматический шум.
* **Многопоточность на чистом C++11:** Высокоскоростные параллельные вычисления, встроенные напрямую в стандарт языка и прозрачно 
масштабируемые на любые архитектуры процессоров.




## Безграничная точность (Arbitrary Precision Arithmetic)

Движок полностью избавлен от аппаратных ограничений стандартных типов данных — нативного 64-битного (`double`) 
и расширенного 80-битного (`long double`), которые неизбежно теряют значащие разряды и приводят к пикселизации 
кадра («эффекту квадратов») на масштабах приближения глубже $10^{-15}$ и $10^{-19}$.

* **Интеграция MPFR/GMP:** Вся высокоточная координатная навигация, масштабирование при кликах мыши и расчет шага 
сетки осуществляются внутри изолированной бинарной памяти с точностью **1000 бит**.
* **Физический лимит в 308 знаков:** Экспоненциальный диапазон аппаратного регистра `double` ограничивает скоростной 
дельта-движок максимальной теоретической глубиной погружения в **$10^{-308}$ знаков**. Текущие координаты кадра 
сохраняются и считываются через файл `Mandelbrot.txt`, гарантируя безупречную четкость математических микроструктур 
на пороге кремниевого барьера.



## Реактивный метод возмущений (Perturbation Theory)

Рендеринг экстремальных приближений больше не требует выполнения ресурсоемких длинных вычислений произвольной точности 
для каждого отдельного пикселя, что исторически замедляло фрактальный софт в тысячи раз.

* **Высокоточная опорная орбита:** Сверхтяжелый BigFloat-радар MPFR рассчитывает абсолютную и точную траекторию всего 
для **одной центральной точки кадра и строго один раз** перед началом рендеринга.
* **Аппаратное ускорение вычислений:** Весь остальной массив экрана (миллионы пикселей) просчитывается параллельно на нативной 
скорости процессорных регистров `double`. Алгоритм вычисляет лишь микроскопические отклонения (дельты) от центральной опорной оси, 
что обеспечивает колоссальный выигрыш в производительности и ускоряет генерацию кадра до 1000 раз!




## Динамическая релокализация опорной точки (Перебазирование орбиты)

Это предмет особой гордости: движок работает на тех же математических и архитектурных принципах, 
что и самые продвинутые рендереры Мандельброта в мире.

* **Динамический сброс в ноль (Dynamic Reset to Zero)**: На каждом шаге для каждого пикселя отслеживается соотношение его абсолютных 
координат и текущего отклонения (дельты). Если дельта уходит слишком далеко или основная опорная орбита завершается, поток 
вычислений динамически сбрасывает индекс отслеживания в ноль. Это превращает накопленные координаты в новое автономное начало 
отсчета и полностью устраняет артефакты точности.
* **Оптимизация цикла <На один шаг за предел ухода> (One-Step Beyond Escape Loop Optimization)**: Чтобы выжать максимум производительности 
из CPU, трекер опорной орбиты MPFR записывает ровно одну дополнительную точку в массив орбиты сразу после того, как она превышает 
радиус ухода (escape radius). Такое точное определение границы гарантирует безупречное перебазирование орбиты.
* **Оптимизация ветвлений в глубоком цикле (Deep Loop Branch Optimization)**: Эта элегантная структура кода существенно упрощает 
условия внутри самого глубокого цикла итераций. Минимизируя тяжелые ветвления и избегая сложных вложенных условий там, где это 
возможно, код повышает эффективность предсказателя ветвлений процессора и позволяет конвейеру CPU выполнять базовые математические 
расчеты с максимальной пропускной способностью.



## Билинейная аппроксимация (Bilinear Approximation)

Расчет фрактала выполняется существенно быстрее за счет интеграции метода билинейной аппроксимации (**BLA Skip-100**). 

* **Предварительный расчет матриц:** Движок заранее вычисляет коэффициенты трансформации матриц для шагов 
по 100 итераций.
* **Масштабный пропуск итераций:** Если текущая точка дельты находится в пределах безопасного радиуса сходимости 
относительно опорной орбиты, алгоритм «перепрыгивает» сразу 100 итераций за один математический шаг. 
Это кардинально снижает нагрузку на процессор и обеспечивает колоссальный прирост скорости рендеринга 
на гладких участках множества Мандельброта.


## Многопоточность на чистом C++11 (Pure C++11 Multi-threading)

Высокоскоростные параллельные вычисления, встроенные напрямую в стандарт языка. 
Алгоритм прозрачно масштабируется на любые архитектуры, обеспечивая безупречную 
100%-ю загрузку процессора как на 4-ядерном ноутбуке, так и на 128-ядерном 
высокопроизводительном сервере - абсолютно без внешних зависимостей.


## Истинное сглаживание SSAA 8x8: 64-точечная интеграция в RGB-пространство

Для достижения беспрецедентного визуального качества в движке реализован алгоритм суперсэмплинга **True 8x8 SSAA**, 
где каждый пиксель формируется на основе **64 независимых субпиксельных координат**. Это позволяет 
устранить алиасинг, возникающий из-за того, что микронити множества Мандельброта меньше физического пикселя.

### Математическая несостоятельность стандартного подхода
Классический подход ошибочно усредняет сырые значения итераций до окрашивания:
* `Color( (iter_1 + iter_2 + ... + iter_64) / 64 )`

Поскольку палитры нелинейны, это проецирует результат на случайный цвет, вызывая ложные хроматические контуры и артефакты.

### Инженерное решение: 64-точечная оптическая интеграция в RGB
Движок вычисляет цвет для каждого из 64 субпикселей независимо до смешивания, выполняя поканальное накопление:
* `(Color(iter_1) + Color(iter_2) + ... + Color(iter_64)) / 64`

### Ключевые технические преимущества:
* **Фрактальное сэмплирование:** Захват микроструктур, меньших размера пикселя.
* **Устранение шума:** Ликвидация хроматического шума за счет работы с каналами RGB.
* **Кинематографическая целостность:** Обеспечение плавных градиентов и высокого качества графики.



## Генерация 255 кадров: стратегия оптимизации

В движке реализована высокоэффективная стратегия предварительного рендеринга: тяжелые математические расчеты (номера итераций) 
выполняются строго один раз, сырые данные сохраняются в буфер, после чего кадры анимации генерируются мгновенно за счет 
циклического сдвига цветов и поканального понижения разрешения (даунсэмплинга).
Поскольку вычисление фрактала в разрешении 17280x17280 целых 255 раз крайне ресурсоемко для процессора, мы разделили эту задачу 
на два независимых этапа.

### Этап 1: Генерация карты итераций (сырые данные)
Вместо создания сотен тяжелых файлов BMP движок создает единый массив данных в оперативной памяти, где для каждого субпикселя сохраняется 
только конечное число итераций (t).
* Для сетки 17280x17280 при использовании типа данных `uint8_t` размер этого буфера составляет приблизительно 298 МБ.

### Этап 2: Рендеринг 255 кадров (цвет + сглаживание)
Программа считывает готовую карту итераций и выполняет следующие операции для каждого кадра последовательности:
* **Понижение разрешения:** Потоки последовательно обрабатывают блоки субпикселей размером 8x8 из высокоразрешенной карты.
* **Цветовое отображение:** Каждое считанное значение проецируется на динамически сдвинутую цветовую палитру кадра.
* **Сглаживание:** Вычисляется среднее оптическое значение цветов (суперсэмплинг SSAA 8x8) для формирования финального 
кадра в разрешении 2160x2160.

### Почему это работает так быстро?
* **Эффективность памяти:** Массив `iterMap` (~298 МБ) целиком и без проблем помещается в ОЗУ современных систем. Тяжелейший цикл 
вычислений метода возмущений запускается ровно ОДИН раз за всю сессию анимации.
* **Вращение палитры:** Второй этап полностью избавлен от длинной арифметики, возведений в квадрат и комплексных чисел. 
Он включает в себя лишь базовое сложение целых чисел и молниеносный поиск индексов в памяти.
* **Масштабируемость:** Благодаря нашему многопоточному движку на чистом C++11, этот этап идеально параллелится — 
кадры просчитываются всеми доступными ядрами процессора одновременно.
* **Честный даунсэмплинг:** Мы используем физически корректное оптическое усреднение блока 8x8, что обеспечивает колоссальное 
превосходство в четкости микронитей фрактала по сравнению со стандартным программным изменением размера изображений.

После того как все 255 файлов BMP сгенерированы, используется утилита FFmpeg для их финальной сборки в видеофайл.



## Визуальная эстетика

Красный, зеленый и синий каналы рассчитываются с использованием синусоидальных и косинусоидальных волн:
```C++
        pal[a][0] = (uint8_t)round(127.0 + 127.0 * cos(2.0 * PI * a / 255.0)); // Blue
        pal[a][1] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Green
        pal[a][2] = (uint8_t)round(127.0 + 127.0 * sin(2.0 * PI * a / 255.0)); // Red
```


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
*    Batch Processing: Работает полностью в автоматическом режиме через командную строку. Вы выбираете точку (1-9), и 
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



## Управление и выбор локаций (CLI Controls)

Поскольку это консольное приложение, управление осуществляется через ввод номера локации при запуске программы.

| Действие | Ввод | Описание |
| :--- | :--- | :--- |
| **Пресеты** | `1` - `9` + `ENTER` | Выбор одной из 9 встроенных точек мандельброта - глубокого зума. |
| **Своя точка** | `0` + `ENTER` | Загрузка координат (`absc`, `ordi`, `size`) из файла `Mandelbrot.txt`. |
| **Рендеринг (Авто)** | **Шаг 1** | Расчет 255 кадров с использованием **Многопоточность** и **SSAA 8x8**. |
| **Выход** | `Любая клавиша` + `ENTER` | Завершение работы программы. |



## Структура файла Mandelbrot.txt

Для загрузки пользовательских координат (пункт 0 в меню), создайте текстовый файл Mandelbrot.txt в папке с программой. 
Файл должен содержать три числа, разделенных переносом строки:
*    Abscissa (Координата X центра)
*    Ordinate (Координата Y центра)
*    Size (Масштаб/Размер области)

Вот так:
```C++
-1.758008286774384722
-0.012039512139649579
7.3e-15
```


## Множество Мандельброта: Математический абсолют

Это поистине один из немногих объектов, который связывает нас с чем-то абсолютно объективным и бесконечным,
превосходящим биологию и историю. Даже если бы вся наша Вселенная и все её атомы исчезли завтра,
уравнение осталось бы верным. Оно не <написано> на звёздах; оно заложено в самой структуре логики.
Это делает множество Мандельброта своего рода абсолютом.
 
Множество Мандельброта существует независимо от нашего разума и технологий. 
Это бесконечная математическая структура, которая существовала всегда.


## Посмотрите на результаты! Невероятная плавность работы

```C++
case 1: absc_str = "-1.74919762896578937419423768162729211653261585574161734756645570271259437106847945278504682550816934045282325615313706"; 
ordi_str = "-0.0000004253077715244042272585501215924940115095651514128365051648578746521975467051762910002043554114332125868777767"; 
size_str = "1.34e-113"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%201.jpg)

```C++
case 2: absc_str = "-1.74907816150520173167912454515663360420734509948112463480292338384"; 
ordi_str = "-0.00000550991906629096602513098567268615714673236269915508056068145"; 
size_str = "1.53e-62"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%202.jpg)

```C++
case 3: absc_str = "-1.7489436617686633372073552153211507258063533373824414679761"; 
ordi_str = "-0.0000073748967541889836640985849393311615399776865199722998"; 
size_str = "1.01e-55"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%203.jpg)

```C++
case 4: absc_str = "-1.7491311840575335110236048528001036247123430447933925298694915282522178938437759580179"; 
ordi_str = "-0.001996960702541036804654299663680287246637758588467627907752429037173153157138373325"; 
size_str = "2.84e-82"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%204.jpg)

```C++
case 5: absc_str = "-1.74994586497557459407526067070055710001"; 
ordi_str = "-0.0000000852088539604644334731909824511"; 
size_str = "7.17e-36"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%205.jpg)

```C++
case 6: absc_str = "-1.267078059171397835210199054200436920994876769284288837862647"; 
ordi_str = "-0.123788215196292957558264285607075473360968832625384429809391"; 
size_str = "2.4e-57"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%206.jpg)

```C++
case 7: absc_str = "-1.99999543561201124623198345433951143502785679245726844745821388800402678499411681518036306219179273434395557574279985918047221291197081186140687781560831995";
ordi_str = "-0.00000000000000000000000026198152173811047783694060060607013913873144250985383083459221663448338433592617272786772587281530484110756597337683912309313885172";
size_str = "1.15e-119"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%207.jpg)

```C++
case 8: absc_str = "-0.843233764530751513324611730003844788663891317894291476942";
ordi_str = "0.223818041558069959139898295480311440395574821870538878074";
size_str = "2.66e-52"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%208.jpg)

```C++
case 9: absc_str = "-0.12702288604303376058504940552835636145639339878096842408417373730142";
ordi_str = "-0.98728474138900681012772182210671973643707041031463043446236865072278";
size_str = "6.18e-64"; break;
```
![Mandelbrot Set](Mandelbrot%20Set%20Image%209.jpg)



**[Скачать последнюю версию (Windows и Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**

