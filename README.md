# 🎧 Al Ahly Audio Player v2.0

A dual-deck DJ audio player built with C++ and the **JUCE** framework. This application was developed as a project for [**Your Course Here**] under the supervision of [**Your Supervisor Here**]. The program allows users to load two audio tracks, mix between them with a crossfader, control playback, and manage a playlist.

## ✨ Features

The application comes with a wide range of professional audio features:

| Feature # | Feature Name | Description |
| :---: | :--- | :--- |
| 1 | **Dual-Deck Player** | Load and control two audio tracks independently (Player 1 & Player 2). |
| 2 | **Crossfader** | A horizontal slider to mix smoothly between the two decks. |
| 3 | **Mix Mode** | A toggle to allow both decks to play simultaneously or only one at a time. |
| 4 | **Playlist** | Add, delete, and clear a list of audio files. Click any track to load it into Player 1. |
| 5 | **Playback Controls** | Full transport controls per deck: Play/Pause, Stop, Skip Forward (+10s), and Back (-10s). |
| 6 | **Audio Controls** | Independent vertical sliders for **Volume** and **Playback Speed** (0.5x to 2.0x). |
| 7 | **Looping Engine** | Standard track looping and a precision **A-B Loop** to repeat a specific section. |
| 8 | **Cue Points** | Set a "Cue" marker on the waveform and instantly jump back to it. |
| 9 | **Waveform Display** | A visual rendering of the audio, allowing you to see the track's structure and click to seek. |
| 10 | **Metadata Reader** | Automatically reads and displays the Title, Artist, and Duration from the audio file (using **TagLib**). |
| 11 | **Session Persistence** | Remembers the last loaded track and its position for each deck when you restart the app. |
| 12 | **Light/Dark Theme** | Toggle the user interface between a light mode and a dark mode. |

---

## 🚀 Getting Started

Follow these steps to get the project running on your local machine.

### Prerequisites

* A C++ compiler (e.g., MSVC, Xcode, or G++).
* The **JUCE Framework** (v6 or later).
* The **TagLib** library (for reading audio metadata). You must have this library compiled and ready to be linked.

### Running the Application

This is a JUCE project and must be compiled using the Projucer.

1.  **Clone the repository** to your local machine.
2.  **Install TagLib** and note the path to its `include` and `lib` files.
3.  **Open the `.jucer` file** in the Projucer.
4.  **Configure Paths:**
    * Go to the **Exporters** tab (e.g., "Visual Studio 2022").
    * In **Header Search Paths**, add the path to your TagLib `include` folder.
    * In **Extra Library Search Paths**, add the path to your TagLib `lib` folder.
5.  **Save and Open in IDE:** Click the "Save and Open in IDE..." button.
6.  **Compile the code.** Build the project from within your IDE (Visual Studio, Xcode, etc.).
7.  **Run the executable.** After successful compilation, run the program from your IDE or find the built executable.

### How to Use:

* When the program starts, both decks are empty.
* Use the **"+"** button in the playlist section to add audio files.
* Click a track in the playlist to load it into **Player 1**.
* Use the **"Open"** button on **Player 2** to load a track into that deck.
* Use the play buttons and the crossfader to begin mixing.

---

## 🖼️ Supported Formats

The program supports loading and saving the following audio formats:
* MP3
* WAV
* FLAC
* OGG

---

## 🧑‍💻 Authors

This project was a collaborative effort by the following students:

* **[Your Name / ID Here]**
* **[Your Partner's Name / ID Here]**

Repository Link: [**https://github.com/YourUsername/YourRepo**]

---
---

# 🎧 مشغل الأهلي الصوتي v2.0

مشغل صوتي احترافي مزدوج (Double Deck) تم تطويره باستخدام لغة C++ وإطار عمل **JUCE**. تم تطوير هذا التطبيق كمشروع لمادة [**اسم المادة هنا**] تحت إشراف [**اسم المشرف هنا**]. يتيح البرنامج للمستخدمين تحميل مقطعين صوتيين، والخلط بينهما، والتحكم بالتشغيل، وإدارة قائمة تشغيل.

## ✨ الميزات

يحتوي البرنامج على مجموعة واسعة من الميزات الصوتية الاحترافية:

| رقم الميزة | اسم الميزة | الوصف |
| :---: | :--- | :--- |
| 1 | **مشغل مزدوج** | تحميل والتحكم في مقطعين صوتيين بشكل مستقل (المشغل 1 والمشغل 2). |
| 2 | **مفتاح الخلط (Crossfader)** | شريط تمرير أفقي للخلط بسلاسة بين المشغلين. |
| 3 | **وضع المزج (Mix Mode)** | زر للسماح بتشغيل كلا المشغلين في وقت واحد أو مشغل واحد فقط. |
| 4 | **قائمة التشغيل (Playlist)** | إضافة وحذف ومسح قائمة من الملفات الصوتية. اضغط على أي مقطع لتحميله في المشغل 1. |
| 5 | **عناصر التحكم** | تحكم كامل في التشغيل لكل مشغل: تشغيل/إيقاف مؤقت، إيقاف، تقديم (+10 ث)، ترجيع (-10 ث). |
| 6 | **التحكم بالصوت** | أشرطة تمرير عمودية مستقلة لـ **مستوى الصوت** و **سرعة التشغيل** (من 0.5x إلى 2.0x). |
| 7 | **نظام التكرار (Looping)** | تكرار عادي للمقطع، وتكرار دقيق بين نقطتين **A-B Loop**. |
| 8 | **نقاط الإشارة (Cue Points)** | ضع علامة "Cue" على الموجة الصوتية واقفز إليها فورًا. |
| 9 | **عرض الموجة الصوتية** | عرض مرئي للملف الصوتي يتيح لك رؤية هيكل المقطع والنقر للبحث. |
| 10 | **قارئ البيانات (Metadata)** | يقرأ ويعرض تلقائيًا العنوان، الفنان، ومدة المقطع من الملف (باستخدام **TagLib**). |
| 11 | **حفظ الجلسة** | يتذكر آخر مقطع تم تحميله وموقعه لكل مشغل عند إعادة تشغيل التطبيق. |
| 12 | **السمات (Themes)** | التبديل بين الوضع الفاتح والوضع المظلم لواجهة المستخدم. |

---

## 🚀 كيفية البدء

اتبع الخطوات التالية لتشغيل المشروع على جهازك.

### المتطلبات الأساسية

* مترجم C++ (مثل MSVC, Xcode, أو G++).
* إطار عمل **JUCE** (الإصدار 6 أو أحدث).
* مكتبة **TagLib** (لقراءة بيانات الملفات الصوتية)، يجب أن تكون مجمعة (compiled) وجاهزة للربط.

### التشغيل

هذا مشروع JUCE ويجب تجميعه (compile) باستخدام Projucer.

1.  **تحميل المشروع:** قم بنسخ المستودع إلى جهازك المحلي.
2.  **تثبيت TagLib:** قم بتثبيت المكتبة واحتفظ بمسار مجلدي `include` و `lib` الخاصين بها.
3.  **فتح ملف `.jucer`:** افتح الملف في Projucer.
4.  **إعداد المسارات:**
    * اذهب إلى تبويب **Exporters** (مثال: "Visual Studio 2022").
    * في **Header Search Paths**، أضف مسار مجلد `include` الخاص بـ TagLib.
    * في **Extra Library Search Paths**، أضف مسار مجلد `lib` الخاص بـ TagLib.
5.  **حفظ وفتح في IDE:** اضغط على زر "Save and Open in IDE...".
6.  **ترجمة الكود (Compile):** قم ببناء المشروع من داخل بيئة التطوير (IDE) الخاصة بك (مثل Visual Studio أو Xcode).
7.  **تشغيل البرنامج:** بعد انتهاء الترجمة بنجاح، قم بتشغيل البرنامج.

### الاستخدام:

* عند بدء التشغيل، ستكون كلا المشغلات فارغة.
* استخدم زر **"+"** في قسم قائمة التشغيل لإضافة ملفات صوتية.
* اضغط على مقطع في القائمة لتحميله في **المشغل 1**.
* استخدم زر **"Open"** في **المشغل 2** لتحميل مقطع في ذلك المشغل.
* استخدم أزرار التشغيل ومفتاح الخلط (Crossfader) لبدء المزج.

---

## 🖼️ الصيغ المدعومة

البرنامج يدعم تحميل وحفظ الصور بالصيغ التالية:
* MP3
* WAV
* FLAC
* OGG

---

## 🧑‍💻 المؤلفون

هذا المشروع هو نتاج عمل جماعي قام به الطلاب:

* **[اسمك / رقمك الأكاديمي هنا]**
* **[اسم زميلك / رقمه الأكاديمي هنا]**

رابط المستودع: [**https://github.com/YourUsername/YourRepo**]
