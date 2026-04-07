# Mandelbrot set. 80-bit long double. OpenMP. Supersampling 8x8 (64 passes). Color rotation

[![English](https://img.shields.io/badge/Language-English-blue)](#english)
[![Русский](https://img.shields.io/badge/Язык-Русский-red)](#russian)

![Language](https://img.shields.io/badge/Language-CPP-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4)
![Precision](https://img.shields.io/badge/Precision-80--bit-red)
![Engine](https://img.shields.io/badge/Engine-OpenMP-orange)
![Graphics](https://img.shields.io/badge/Graphics-SSAA-brightgreen)

[![Download](https://img.shields.io/badge/Download-Mandelbrot.exe-brightgreen?logo=windows)](https://github.com/Divetoxx/Mandelbrot/releases/latest/download/Mandelbrot.exe)
[![Source Code](https://img.shields.io/badge/Source_Code-ZIP-orange?logo=github)](https://github.com/Divetoxx/Mandelbrot/archive/refs/heads/main.zip)


<a name="english"></a>
## 🇺🇸 English Version

## Project Purpose
This application is a high-performance command-line (CLI) utility designed for rendering high-fidelity frame sequences of the Mandelbrot set. 
It focuses on precision and offline rendering rather than real-time navigation.

## Key Features:
*    Animation Ready (Frame Sequences): The utility automatically generates a sequence of 255 BMP frames with a smooth palette rotation 
effect. These frames are ready to be encoded into high-quality video (e.g., via FFmpeg).
*    Extreme SSAA 8x8: Implements a massive 8x8 Super Sampling Anti-Aliasing. Each final pixel is derived from 64 independent 
sub-samples, resulting in unparalleled image clarity and zero aliasing noise.
*    4K Ultra HD+ Resolution: Pre-configured for 3840x3840 output. Combined with the 8x8 sampling, it produces details far beyond standard 
fractal visualizers.
*    CLI Automation: Choose a preset location (1-9) or load custom coordinates from Mandelbrot.txt, and let the engine 
handle the heavy math and file I/O.

## Use Cases:
*    Video Content Creation: The primary tool for generating high-resolution fractal zoom videos and psychedelic animations.
*    CPU Stress Testing: Utilizing OpenMP, it pushes all CPU cores to their limits with massive parallelized `long double` computations.
*    Mathematical Photography: Captures the deepest parts of the Mandelbrot set (up to $10^{18}$) with 80-bit precision.

## One-Shot Variety: 
*    The utility generates 255 different color variations for your chosen location in a single run. Instead of re-rendering to find the perfect 
look, you can simply browse the output folder and pick the most aesthetically pleasing frame. Using the Palette Shifting method, 
the heavy fractal math is calculated only once, while all 255 variations are produced almost instantly.

## Controls & Preset Selection (CLI)
Since this is a command-line interface (CLI) application, navigation is handled by entering the location number at program startup.

| Action | Input | Description |
| :--- | :--- | :--- |
| **Presets** | `1` - `8` | Instantly select one of the 8 unique deep-zoom locations. |
| **Custom Point** | `9` | Load coordinates (`absc`, `ordi`, `size`) from `Mandelbrot.txt`. |
| **Exit** | `Any other` | Safely close the application. |


```C++
case 1: absc = -1.749675773048651182L; ordi = -0.000001140170813768L; size_val = 0.0000000000000021L; break;
case 2: absc = -0.1544283964364377L; ordi = -1.03085800754665175L; size_val = 0.000000000000027L; break;
case 3: absc = -1.749949182103598356L; ordi = -0.000000005697456381L; size_val = 0.0000000000000082L; break;
case 4: absc = -1.7499458023023889L; ordi = -0.00000000065777L; size_val = 0.00000000000013L; break;
case 5: absc = -1.74907816150389628L; ordi = 0.00000550988750089L; size_val = 0.0000000000000015L; break;
case 6: absc = -1.785772653736032933L; ordi = 0.000000500077787345L; size_val = 0.0000000000000077L; break;
case 7: absc = 0.1240478091400506L; ordi = 0.6574314876275071L; size_val = 0.000000000000095L; break;
case 8: absc = -1.78577278039667471L; ordi = -0.00000075696313293L; size_val = 0.0000000000000022L; break;
```

![Mandelbrot txt](Mandelbrot.png)

**[Download Latest Version (Windows & Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**


## Optional: Rendering a Video
If you want to see how these colors flow, you can compile all 255 frames into a video (30 FPS) using FFmpeg.
You can download the pre-compiled FFmpeg binary from my repository:

**[Download FFmpeg Here](https://github.com/Divetoxx/Mandelbrot-Video/releases)**

Use the following command to encode the frames into a Mandelbrot.mp4 file: 

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v libx264 -refs 6 -me_method umh -partitions all -psy 0 -qp 18 -subq 9 -me_range 24 -deblock -6:-6 -bf 6 -i_qfactor 2 -trellis 0 -b_strategy 2 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

If you have an NVIDIA graphics card, you can significantly speed up the encoding process:

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v h264_nvenc -b:v 50M -profile:v high -coder 1 -rc-lookahead 32 -color_range full -pix_fmt yuv420p Mandelbrot.mp4



## High-Precision Rendering (80-bit)
Most Mandelbrot explorers use standard **64-bit double precision**, which leads to "pixelation" at zoom levels around $10^{14}$. 
This project leverages **80-bit Extended Precision Arithmetic** (`long double`) to push the boundaries of the fractal:
*   **My Implementation (80-bit):** Provides **4 extra decimal digits** of precision, allowing you to explore **10,000x deeper** ($10^{18}$ range).
*   **Hardware Optimized:** Directly utilizes the **x87 FPU registers** for maximum mathematical depth.


## OpenMP
OpenMP is a standard that tells the compiler, "Take this loop and distribute the iterations among the different processor cores."
Yes, using OpenMP you are doing parallel programming at the Multithreading level.
Everything is powered by **OpenMP** parallel loops for maximum performance.
OpenMP - Scalability: Your code will run equally efficiently on a 4-core laptop and a 128-core server.


## 8x8 Supersampling (64 Samples Per Pixel)
Super-Sampling Anti-Aliasing (SSAA) is a high-end technique increasing samples per pixel to enhance image quality, 
with 8x (N=8) rendering scenes at 8x resolution on both axes to produce 64 samples per pixel. 
This process calculates an extreme number of pixels-scaling to a 30720 x 30720 grid for a 3840 x 3840
target-before downscaling to remove jaggies and improve detail.

I decided to take the visual quality to a completely different level. This engine implements
True 8x8 Supersampling Anti-Aliasing (SSAA) with 64 independent samples per single screen pixel, utilizing Direct RGB-Space Integration.
Instead of a standard 3840x3840 render, the engine internally processes a massive 30720 x 30720 sub-pixel grid!
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

Since calculating a 30720x30720 fractal 255 times is computationally expensive, we split the task into two stages.

Stage 1: Iteration Map Generation (Raw Data)

Instead of BMP files, we create a single data buffer where we store only the iteration number (t) for each pixel.
*    For 30720x30720 using uint8_t, the resulting file/buffer is approximately 900 MB.

Stage 2: 255-Frame Rendering (Color + Anti-aliasing)

We read the iteration map and perform the following for each frame:
*    Downsample: Process an 8x8 pixel block from the high-res map.
*    Color Mapping: Map each pixel value to a shifted color palette.
*    Smoothing: Average the colors (Supersampling Anti-Aliasing) to produce a final 3840x3840 frame.

Why is this so fast?
*    Memory Efficiency: The iterMap array (~900 MB) easily fits into modern RAM. The heavy do-while calculation loop runs only once for the entire animation.
*    Palette Rotation: Stage 2 avoids long double arithmetic and squaring. It only involves integer addition and memory lookups.
*    Parallelism: Stage 2 is perfectly scalable. All 255 frames can be rendered simultaneously across CPU cores.
*    True Downsampling: We implement honest 8x8 averaging, resulting in superior image quality compared to simple resizing.


## Visual Aesthetics
The Red, Green, and Blue channels are calculated using sine and cosine waves to create smooth color transitions:
127 + 127 * cos(2 * PI * a / 255) and 127 + 127 * sin(2 * PI * a / 255).


## Look at the results! The smoothness is incredible 

![Mandelbrot Set](Mandelbrot%20Set%20Image%201.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%202.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%203.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%204.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%205.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%206.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%207.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%208.jpg)


## The Mandelbrot Set: A Mathematical Absolute

The Mandelbrot Set. It is perfect — an immaterial origin existing outside of space and time. 
No matter who or where the observer is, even an alien a hundred million light-years away, the Mandelbrot Set remains the same. 
Even in a different century, in a different galaxy, and even with a completely different brain, the set is identical. 
It transcends everything, bypassing billions of light-years.

This is not a human invention, but a mathematical discovery. It belongs to the category of "eternal truths" 
that Plato referred to as the Realm of Ideas. This is why it remains constant for any observer in the universe:
*   **Pure Logic**: It is generated by a simple formula. The rules of arithmetic are universal. Any intelligence would inevitably arrive at the exact same fractal boundaries.
*   **Substrate Independence**: This set doesn't need a computer or a human brain to exist. It is an abstract structure woven into the very logic of the cosmos.
*   **Fractal Constancy**: Even if physical constants were different in another galaxy, the mathematical topology of this object would remain unshakable.

It is truly one of the few objects that connects us to something absolutely objective and infinite, 
transcending biology and history. Even if our entire universe and all its atoms were to vanish tomorrow, 
the equation would remain true. It is not "written" on the stars; it is embedded in the structure of logic itself. 
This makes the Mandelbrot Set a kind of absolute.

This is classic Mathematical Platonism: the idea that mathematical objects exist in reality, but in a non-material realm. 
If all matter were to disappear, there would be no one to write down the formula or witness its visualization, 
but the relationship between the numbers would remain true. Much like "2 + 2 = 4", this rule doesn't need apples 
or stones to be valid.

In this sense, truth is primary to the physical world.

The Mandelbrot Set is absolutely predetermined. Every single one of its points was already 'there' long before the Big Bang. 
Yet, at the same time, it is entirely unpredictable-you cannot know what you will see in the next zoom until you perform the calculation.

Looking at a fractal, we witness an incredible complexity that appears chaotic. But we know that at its core lies a formula 
of just three symbols. This makes one wonder: could all the chaos of our universe-the turbulence of water, the formation of clouds, 
the structure of galaxies-be nothing more than the result of a very simple algorithm that we have yet to calculate?




<a name="russian"></a>
## 🇷🇺 Русская версия

# Множество Мандельброта. 80-бит long double. OpenMP. Суперсэмплинг 8x8 (64 прохода). Динамическая смена палитры

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
*    4K Ultra HD+ Резолюция: По умолчанию настроен рендер в разрешении 3840x3840, что в сочетании с SSAA 8x8 дает 
качество, недоступное обычным real-time приложениям.
*    Batch Processing: Работает полностью в автоматическом режиме через командную строку. Вы выбираете точку (1-9), и 
программа выполняет всю тяжелую работу по расчету и сохранению файлов.

## Для чего это нужно:
*    Создание видео-арта: Идеально подходит для создания "бесконечных" психоделических зумов и фрактальных анимаций 
в высоком разрешении.
*    Стресс-тестирование CPU: Благодаря OpenMP, программа загружает все ядра процессора на 100%, выполняя миллиарды вычислений 
с плавающей запятой (`long double`).
*    Глубокое исследование: Поддержка 80-битной точности позволяет "фотографировать" участки фрактала на глубине до $10^{18}$.

## Мгновенная вариативность: 
*    Программа генерирует 255 различных вариантов раскраски для выбранной локации за один проход. Вам не нужно запускать рендер 
снова и снова, чтобы подобрать идеальный вид - просто откройте папку и выберите самый красивый кадр из готовой серии. 
Благодаря методу Palette Shifting, расчет математики происходит один раз, а все 255 изображений создаются практически мгновенно.


## Управление и выбор локаций (CLI Controls)
Поскольку это консольное приложение, управление осуществляется через ввод номера локации при запуске программы.

| Действие | Ввод | Описание |
| :--- | :--- | :--- |
| **Пресеты** | `1` - `8` | Мгновенный выбор одной из 8 уникальных точек глубокого зума. |
| **Своя точка** | `9` | Загрузка координат (`absc`, `ordi`, `size`) из файла `Mandelbrot.txt`. |
| **Выход** | `Любая клавиша` | Завершение работы программы. |


```C++
case 1: absc = -1.749675773048651182L; ordi = -0.000001140170813768L; size_val = 0.0000000000000021L; break;
case 2: absc = -0.1544283964364377L; ordi = -1.03085800754665175L; size_val = 0.000000000000027L; break;
case 3: absc = -1.749949182103598356L; ordi = -0.000000005697456381L; size_val = 0.0000000000000082L; break;
case 4: absc = -1.7499458023023889L; ordi = -0.00000000065777L; size_val = 0.00000000000013L; break;
case 5: absc = -1.74907816150389628L; ordi = 0.00000550988750089L; size_val = 0.0000000000000015L; break;
case 6: absc = -1.785772653736032933L; ordi = 0.000000500077787345L; size_val = 0.0000000000000077L; break;
case 7: absc = 0.1240478091400506L; ordi = 0.6574314876275071L; size_val = 0.000000000000095L; break;
case 8: absc = -1.78577278039667471L; ordi = -0.00000075696313293L; size_val = 0.0000000000000022L; break;
```

![Mandelbrot txt](Mandelbrot.png)

**[Скачать последнюю версию (Windows и Linux)](https://github.com/Divetoxx/Mandelbrot/releases)**


## Дополнительно: Рендеринг видео
Если вы хотите увидеть, как эти цвета перетекают, вы можете скомпилировать все 255 кадров в видео (30 кадров в секунду) с помощью FFmpeg.
Вы можете скачать предварительно скомпилированный бинарный файл FFmpeg из моего репозитория:

**[Скачать FFmpeg здесь](https://github.com/Divetoxx/Mandelbrot-Video/releases)**

Используйте следующую команду для кодирования кадров в файл Mandelbrot.mp4: 

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v libx264 -refs 6 -me_method umh -partitions all -psy 0 -qp 18 -subq 9 -me_range 24 -deblock -6:-6 -bf 6 -i_qfactor 2 -trellis 0 -b_strategy 2 -color_range full -pix_fmt yuv420p Mandelbrot.mp4

Если у вас видеокарта NVIDIA, вы можете значительно ускорить процесс кодирования:

ffmpeg -y -stream_loop 3 -framerate 30 -i Mandelbrot%%03d.bmp -bsf:v h264_metadata=video_full_range_flag=0 -c:v h264_nvenc -b:v 50M -profile:v high -coder 1 -rc-lookahead 32 -color_range full -pix_fmt yuv420p Mandelbrot.mp4



## Высокоточная отрисовка (80-бит)
Большинство исследователей фрактала Мандельброта используют стандартную **64-битную двойную точность**,
что приводит к "пикселизации" при масштабировании около $10^{14}$.
В этом проекте используется **80-битная арифметика с расширенной точностью** (`long double`) для расширения границ фрактала:
*  **Моя реализация (80-бит):** Обеспечивает **4 дополнительных десятичных знака** точности, позволяя исследовать **в 10 000 раз глубже** (диапазон $10^{18}$).
*  **Аппаратная оптимизация:** Непосредственно использует **регистры FPU x87** для максимальной глубины математических вычислений.


## OpenMP
OpenMP - это стандарт, который говорит компилятору: "Возьми этот цикл и сам раздай итерации разным ядрам процессора".
Используя OpenMP, вы занимаетесь параллельным программированием на уровне многопоточности (Multithreading).
OpenMP - масштабируемость: ваш код будет одинаково эффективно работать как на 4-ядерном ноутбуке,
так и на 128-ядерном сервере.


## Суперсэмплинг 8x8 (64 прохода на один пиксель)
Суперсэмплинг (SSAA) - ресурсоемкий метод сглаживания, увеличивающий число выборок на пиксель для повышения качества изображения. 
При значении 8x (N=8) сцена рендерится в разрешении, в 8 раз превышающем целевое, по обеим осям, создавая 64 (или 8 х 8) выборки 
на пиксель. Изображение просчитывается в более высоком разрешении, а затем принудительно уменьшается до разрешения дисплея, 
устраняя лесенки и улучшая чёткость. Это очень высокая нагрузка! Это не 3840 на 3840 пикселя а в 8x8 больше - 30720 на 30720 пикселя!

Я решил вывести качество изображения на совершенно новый уровень. Этот движок использует
истинное сглаживание 8x8 Supersampling Anti-Aliasing (SSAA) с 64 независимыми сэмплами на каждый пиксель экрана, используя прямую интеграцию в RGB-пространство.
Вместо стандартного рендеринга 3840x3840, движок обрабатывает внутри себя огромную сетку из 30720 x 30720 субпикселей!

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
Поскольку считать 30720x30720 255 раз - это безумие, мы разделим задачу на два этапа.

Этап 1: Генерация <карты итераций> (Raw Data)

Вместо BMP мы создадим один огромный файл, где для каждого пикселя запишем только число t (номер итерации). 
Для 30720x30720 при использовании uint8_t файл займет около 900 МБ.

Этап 2: Генерация 255 кадров (Цвет + Сглаживание)

Теперь мы читаем эту карту и для каждого кадра делаем:
Берем блок 8x8 пикселей из большой карты.
Красим каждый пиксель согласно сдвинутой палитре.
Усредняем цвета (это и есть сглаживание) и записываем в файл 3840x3840.
Почему это сработает быстро?
*    **Память**: Массив iterMap занимает около 900 МБ. Это легко помещается в современную оперативную память. 
Тяжелый цикл do-while выполняется только один раз для всей анимации.
*    **Вращение палитры**: В этапе 2 нет long double, нет возведения в квадрат. Только сложение целых чисел и чтение из памяти.
*    **Параллелизм**: Этап 2 тоже идеально распараллеливается. 255 кадров будут вылетать очень быстро. 
Реализован честный Downsampling. Мы берем блок 8x8 и усредняем их. 


## Визуальная эстетика
Красный, зеленый и синий каналы рассчитываются с использованием синусоидальных и косинусоидальных волн для создания плавных цветовых переходов:
127 + 127 * cos(2 * PI * a / 255) и 127 + 127 * sin(2 * PI * a / 255).


## Посмотрите на результаты! Невероятная плавность работы

![Mandelbrot Set](Mandelbrot%20Set%20Image%201.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%202.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%203.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%204.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%205.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%206.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%207.jpg)
![Mandelbrot Set](Mandelbrot%20Set%20Image%208.jpg)


## Множество Мандельброта: Математический абсолют

Множество Мандельброта. Оно совершенно - нематериальное происхождение, существующее вне пространства и времени.
Неважно, кто и где находится наблюдатель, даже инопланетянин на расстоянии ста миллионов световых лет, множество Мандельброта остается неизменным.
Даже в другом столетии, в другой галактике и даже с совершенно другим мозгом, множество идентично.
Оно превосходит всё, минуя миллиарды световых лет.

Это не человеческое изобретение, а математическое открытие. Оно принадлежит к категории <вечных истин>,
которые Платон называл Царством Идей. Вот почему оно остается неизменным для любого наблюдателя во Вселенной:
*  **Чистая логика**: Оно порождается простой формулой. Правила арифметики универсальны. Любой разум неизбежно придет к одним и тем же фрактальным границам.
*  **Независимость от субстрата**: Для существования этого множества не нужен компьютер или человеческий мозг. Это абстрактная структура, вплетенная в саму логику космоса.
*  **Фрактальная постоянство**: Даже если физические константы в другой галактике будут другими, математическая топология этого объекта останется непоколебимой.

Это поистине один из немногих объектов, который связывает нас с чем-то абсолютно объективным и бесконечным,
превосходящим биологию и историю. Даже если бы вся наша Вселенная и все её атомы исчезли завтра,
уравнение осталось бы верным. Оно не <написано> на звёздах; оно заложено в самой структуре логики.
Это делает множество Мандельброта своего рода абсолютом.

Это классический математический платонизм: идея о том, что математические объекты существуют в реальности, но в нематериальной сфере.
Если бы вся материя исчезла, некому было бы записать формулу или увидеть её визуализацию,
но соотношение между числами осталось бы верным. Подобно правилу <2 + 2 = 4>, этому правилу не нужны яблоки
или камни, чтобы быть действительным.

В этом смысле истина является первостепенной по отношению к физическому миру.

Множество Мандельброта абсолютно предопределено. Каждая его точка была <там> еще до Большого взрыва. 
Но при этом оно абсолютно непредсказуемо - вы не узнаете, что увидите при следующем зуме, пока не сделаете расчет.

Глядя на фрактал, мы видим невероятную сложность, которая кажется хаотичной. 
Но мы знаем, что в её основе лежит формула из трех символов. Это заставляет задуматься: 
а не является ли весь хаос нашей Вселенной - турбулентность воды, рост облаков, структура 
галактик - лишь результатом работы очень простого алгоритма, который мы ещё не вычислили?


