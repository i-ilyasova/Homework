import os
import stat

def fs_info(path):
    """Выводит информацию о файловой системе по указанному пути."""
    try:
        st = os.statvfs(path)
        info = {
            'Тип файловой системы': 'Не определён напрямую в os',
            'Размер блока (байт)': st.f_bsize,
            'Количество блоков': st.f_blocks,
            'Количество свободных блоков': st.f_bfree,
            'Количество доступных блоков': st.f_bavail,
            'Количество inode': st.f_files,
            'Количество свободных inode': st.f_ffree,
            'Флаги файловой системы': st.f_flag,
            'Максимальная длина имени файла': st.f_namemax
        }
        return info
    except Exception as e:
        return f"Ошибка: {str(e)}"

def file_info(filepath):
    """Выводит информацию о файле."""
    try:
        st = os.stat(filepath)
        mode = st.st_mode
        file_type = 'Неизвестно'
        if stat.S_ISREG(mode):
            file_type = 'Обычный файл'
        elif stat.S_ISDIR(mode):
            file_type = 'Каталог'
        elif stat.S_ISCHR(mode):
            file_type = 'Символьное устройство'
        elif stat.S_ISBLK(mode):
            file_type = 'Блочное устройство'
        elif stat.S_ISFIFO(mode):
            file_type = 'FIFO (канал)'
        elif stat.S_ISLNK(mode):
            file_type = 'Символическая ссылка'
        elif stat.S_ISSOCK(mode):
            file_type = 'Сокет'
        attributes = {
            'inode': st.st_ino,
            'тип файла': file_type,
            'права доступа': stat.filemode(mode),
            'размер (байт)': st.st_size,
            'время последнего доступа': st.st_atime,
            'время последней модификации': st.st_mtime,
            'время создания': st.st_ctime
        }
        return attributes
    except Exception as e:
        return f"Ошибка: {str(e)}"

# Примеры вызова
print("Информация о файловой системе корня:")
print(fs_info('/'))
print("\nИнформация о файле /etc/passwd:")
print(file_info('/etc/passwd'))

# Для сравнения с другой файловой системой (например, NTFS или ext3 на ВМ)
# В онлайн-компиляторе обычно доступен только корень, поэтому пример с тем же путём
print("\nИнформация о файловой системе (повторно):")
print(fs_info('/'))
print("\nИнформация о файле (повторно):")
print(file_info('/etc/passwd'))
