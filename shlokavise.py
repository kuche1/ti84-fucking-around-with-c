#! /usr/bin/env python3

str_bg = 'План за управление. Това е всеобхватната равнина, която се занимава с всички операции по администриране, поддръжка, мониторинг на изпълнението, диагностика на грешки, събиране на статистически данни и др.'

SAME = "0123456789., ()"
SAME += 'abcdefghjklmnopqrstuvwxyz'

NON_ASCII_TRANSLATE = {
    # cyrlic
    'а': 'a',
    'б': 'b',
    'в': 'v',
    'г': 'g',
    'д': 'd',
    'е': 'e',
    'ж': 'j',
    'з': 'z',
    'и': 'i',
    # 'й': 'i',
    'к': 'k',
    'л': 'l',
    'м': 'm',
    'н': 'n',
    'о': 'o',
    'п': 'p',
    'р': 'r',
    'с': 's',
    'т': 't',
    'у': 'u', # note
    'ф': 'f',
    'х': 'h',
    'ц': 'c',
    'ч': 'ch', # note
    'ш': 'sh', # note
    # 'щ': 'a',
    'ъ': 'u', # note
    # 'а': 'a', // ер малък
    # 'ю': 'a',
    'я': 'q',
}

str_en = ''
for char in str_bg:
    if (char in SAME) or (char.lower() in SAME):
        pass
    elif char in NON_ASCII_TRANSLATE:
        char = NON_ASCII_TRANSLATE[char]
    elif char.lower() in NON_ASCII_TRANSLATE:
        char = NON_ASCII_TRANSLATE[char.lower()].upper()
    else:
        assert False, f'unknown char `{char}` {ord(char)}'
    str_en += char

print(str_en)