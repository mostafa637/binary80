# Binary80 - Extended Precision Floating Point Library

## .gitignore

```gitignore
# Compiled Object files
*.o
*.obj
*.ko
*.elf

# Compiled Dynamic libraries
*.so
*.dylib
*.dll

# Compiled Static libraries
*.a
*.lib
*.la
*.lo

# Executables
*.exe
*.out
*.app
test_binary80
binary80_test

# Debug files
*.dSYM/
*.su
*.idb
*.pdb

# Editor/IDE files
.vscode/
.idea/
*.swp
*.swo
*~
.DS_Store

# Build directories
build/
dist/
bin/
obj/

# Dependency directories
.deps/

# Core dumps
core
core.*

# Temporary files
*.tmp
*.log
*.bak
```

<div dir="rtl">

مكتبة C لتنفيذ الحسابات بدقة عالية باستخدام صيغة Binary80 (80-bit extended precision).

</div>

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/yourusername/binary80)
[![License](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Language](https://img.shields.io/badge/language-C99-orange.svg)](https://en.wikipedia.org/wiki/C99)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)](https://github.com/yourusername/binary80)

## 📋 المحتويات

- [نظرة عامة](#نظرة-عامة)
- [المميزات](#المميزات)
- [البناء والتثبيت](#البناء-والتثبيت)
- [الاستخدام](#الاستخدام)
- [الأمثلة](#الأمثلة)
- [واجهة برمجة التطبيقات (API)](#واجهة-برمجة-التطبيقات-api)
- [الترخيص](#الترخيص)

## 🔍 نظرة عامة

Binary80 هي مكتبة مكتوبة بلغة C توفر دعمًا كاملاً للعمليات الحسابية بدقة 80-bit (Extended Precision). تدعم المكتبة التحويل بين الأنواع المختلفة والعمليات الحسابية الأساسية والمتقدمة.

### الهيكل

```
binary80/
├── binary80.h          # ملف الهيدر (التصريحات)
├── binary80.c          # ملف التنفيذ
├── test_binary80.c     # ملف الاختبارات
├── Makefile           # ملف البناء
├── LICENSE            # رخصة BSD 3-Clause
├── .gitignore         # ملفات Git المستبعدة
└── README.md          # التوثيق
```

## ✨ المميزات

- ✅ **دقة عالية**: دعم كامل لصيغة Binary80 (80-bit)
- ✅ **عمليات حسابية**: جمع، طرح، ضرب، قسمة
- ✅ **قيم خاصة**: صفر، لا نهاية، NaN (QNaN و SNaN)
- ✅ **التحويل**: تحويل من/إلى float, double, int
- ✅ **المقارنة**: ==, >, <
- ✅ **دوال مساعدة**: 64-bit arithmetic operations
- ✅ **معيار IEEE 754**: متوافق مع المعايير

## 🔧 البناء والتثبيت

### المتطلبات

- GCC أو أي مترجم C متوافق مع C99
- Make (GNU Make مفضل)
- مكتبة الرياضيات القياسية (libm)

### البناء

```
# استنساخ المستودع
git clone https://github.com/yourusername/binary80.git
cd binary80

# البناء
make

# تشغيل الاختبارات
make test
```

### خيارات الترجمة

```
# بناء مع تحسينات
make CFLAGS="-O3 -march=native"

# بناء للتصحيح
make CFLAGS="-g -O0 -Wall -Wextra"

# بناء static library
ar rcs libbinary80.a binary80.o
```

### التنظيف

```
# حذف الملفات المبنية
make clean

# إعادة البناء
make rebuild
```

### التثبيت

```
# تثبيت على النظام (يتطلب صلاحيات root)
sudo make install

# إلغاء التثبيت
sudo make uninstall
```

## 📖 الاستخدام

### مثال بسيط

```c
#include "binary80.h"
#include <stdio.h>

int main() {
    // إنشاء أرقام
    binary80_t a = binary80_from_double(3.14159);
    binary80_t b = binary80_from_double(2.71828);
    
    // عمليات حسابية
    binary80_t sum = binary80_add(a, b);
    binary80_t diff = binary80_sub(a, b);
    binary80_t prod = binary80_mul(a, b);
    binary80_t quot = binary80_div(a, b);
    
    // طباعة النتائج
    printf("a + b = "); binary80_print(sum);
    printf("a - b = "); binary80_print(diff);
    printf("a * b = "); binary80_print(prod);
    printf("a / b = "); binary80_print(quot);
    
    // مقارنة
    if (binary80_gt(a, b)) {
        printf("a > b\n");
    }
    
    return 0;
}
```

### الترجمة اليدوية

```
gcc -Wall -std=c99 binary80.c your_program.c -o your_program -lm
```

## 📚 واجهة برمجة التطبيقات (API)

### إنشاء القيم

```c
binary80_t binary80_from_double(double val);
binary80_t binary80_from_float(float val);
binary80_t binary80_from_int(int val);
binary80_t binary80_zero(int sign);
binary80_t binary80_Inf(int sign);
binary80_t binary80_QNaN(void);
binary80_t binary80_SNaN(void);
```

### التحويل

```c
double binary80_to_double(binary80_t x);
```

### العمليات الحسابية

```c
binary80_t binary80_add(binary80_t a, binary80_t b);  // جمع
binary80_t binary80_sub(binary80_t a, binary80_t b);  // طرح
binary80_t binary80_mul(binary80_t a, binary80_t b);  // ضرب
binary80_t binary80_div(binary80_t a, binary80_t b);  // قسمة
binary80_t binary80_neg(binary80_t x);                // نفي
```

### المقارنة

```c
bool binary80_eq(binary80_t a, binary80_t b);  // ==
bool binary80_gt(binary80_t a, binary80_t b);  // >
bool binary80_lt(binary80_t a, binary80_t b);  // <
```

### الفحص

```c
bool binary80_is_zero(binary80_t x);
bool binary80_is_NaN(binary80_t x);
bool binary80_is_Inf(binary80_t x, int check_sign);
bool binary80_is_QNaN(binary80_t x);
bool binary80_is_SNaN(binary80_t x);
```

### الطباعة

```c
void binary80_print(binary80_t x);        // طباعة القيمة
void binary80_print_bits(binary80_t x);   // طباعة البتات
```

## 🧪 الأمثلة

### مثال 1: حسابات أساسية

```c
binary80_t x = binary80_from_double(10.0);
binary80_t y = binary80_from_double(3.0);
binary80_t result = binary80_div(x, y);
binary80_print(result);  // 3.3333333333333330
كون الناتج
3.3333333333333330
بدلاً من
3.3333333333333335
فهو بسبب عدم وجود تقريب وقد تم ازالتة لتبسيط الكود لكونة كود تعليمى
```

### مثال 2: معالجة القيم الخاصة

```c
binary80_t inf = binary80_Inf(0);
binary80_t zero = binary80_zero(0);

if (binary80_is_Inf(inf, -1)) {
    printf("Infinity detected\n");
}
```

### مثال 3: دقة عالية

```c
binary80_t large = binary80_from_double(1e15);
binary80_t divisor = binary80_from_double(7.0);
binary80_t result = binary80_div(large, divisor);
binary80_print(result);
```

### مثال 4: حسابات متسلسلة

```c
binary80_t result = binary80_from_double(1.0);

for (int i = 1; i <= 100; i++) {
    binary80_t term = binary80_from_int(i);
    result = binary80_mul(result, term);
}

// result الآن يحتوي على 100!
binary80_print(result);
```

## 🏗️ هيكل البيانات

### binary80_t

```c
typedef struct {
    uint64_t significand;  // 64 bits (explicit leading bit)
    uint16_t exponent;     // 15 bits
    uint16_t sign;         // 1 bit
} binary80_t;
```

**المكونات:**
- **Sign**: 1 bit (0 = موجب، 1 = سالب)
- **Exponent**: 15 bits (bias = 16383)
- **Significand**: 64 bits (مع البت الصريح)

**الصيغة:**

```
Value = (-1)^sign × 2^(exponent - 16383) × (significand / 2^63)
```

## 🎯 حالات الاستخدام

- **الحسابات العلمية**: حيث تكون الدقة العالية ضرورية
- **التشفير**: عمليات حسابية على أعداد كبيرة
- **محاكاة FPU**: محاكاة معالج الفاصلة العائمة x87
- **التحقق من الخوارزميات**: التحقق من دقة خوارزميات الأرقام العائمة

## ⚙️ التكامل مع المشاريع

### استخدام كمكتبة ثابتة

```
# في Makefile الخاص بك
LDFLAGS += -L/path/to/binary80 -lbinary80 -lm
CFLAGS += -I/path/to/binary80
```

### استخدام مباشر

```
# نسخ الملفات إلى مشروعك
SOURCES += binary80.c
HEADERS += binary80.h
```

## 🐛 الإبلاغ عن المشاكل

إذا وجدت خطأ أو لديك اقتراح:

1. تحقق من [Issues](https://github.com/yourusername/binary80/issues) الموجودة
2. إذا لم تجد المشكلة، افتح issue جديد
3. قدم أكبر قدر ممكن من المعلومات:
   - نظام التشغيل
   - إصدار المترجم
   - كود مثال يعيد إنتاج المشكلة
   - الإخراج المتوقع والفعلي

## 🤝 المساهمة

المساهمات مرحب بها! يرجى:

1. انسخ المشروع
2. إنشاء branch للميزة (`git checkout -b feature/AmazingFeature`)
3. التأكد من أن الكود يمر جميع الاختبارات (`make test`)
4. Commit التغييرات (`git commit -m 'Add some AmazingFeature'`)
5. Push إلى Branch (`git push origin feature/AmazingFeature`)
6. فتح Pull Request

### إرشادات المساهمة

- اتبع نمط الكود الموجود
- أضف اختبارات للميزات الجديدة
- حدّث التوثيق حسب الحاجة
- تأكد من عدم وجود تحذيرات في الترجمة

## 📝 الترخيص

هذا المشروع مرخص تحت **BSD 3-Clause License** - انظر ملف [LICENSE](LICENSE) للتفاصيل.

```
Copyright (c) 2024, [Your Name]
All rights reserved.
```

يمكنك بحرية:
- ✅ استخدام الكود لأغراض تجارية
- ✅ تعديل الكود
- ✅ توزيع الكود
- ✅ الاستخدام الخاص

بشرط:
- 📄 الاحتفاظ بإشعار حقوق النشر
- 📄 الاحتفاظ برخصة BSD
- ⚠️ عدم استخدام اسم المؤلف للترويج


## شكر وتقدير

- مستوحى من **معيار IEEE 754**
- استخدام خوارزميات من **Go's math/bits package**
- مرجع: **Intel x87 FPU Documentation**

## 🗺️ خارطة الطريق

- [x] العمليات الحسابية الأساسية (جمع، طرح، ضرب، قسمة)
- [x] دعم القيم الخاصة (Inf, NaN)
- [x] عمليات المقارنة
- [ ] دوال رياضية متقدمة (sqrt, sin, cos, exp, log)
- [ ] دعم تقريب مختلف (round to nearest, toward zero, etc.)
- [ ] تحسينات الأداء
- [ ] دعم SIMD
- [ ] Python bindings

---

<div align="center">

**ملاحظة**: هذه المكتبة مخصصة للأغراض التعليمية والتجريبية.  
للاستخدام في الإنتاج، يُنصح بإجراء اختبارات شاملة.

Made with ❤️ for High Precision Computing

</div>


