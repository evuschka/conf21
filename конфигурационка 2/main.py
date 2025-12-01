#!/usr/bin/env python3
"""
Главный модуль приложения - точка входа.
Запускает CLI и обрабатывает результаты выполнения.
"""

import sys
from src.cli import run_cli # type: ignore


def main() -> int:
    """
    Основная функция приложения.
    
    Returns:
        Код завершения (0 - успех, не 0 - ошибка)
    """
    success = run_cli()
    
    if success:
        return 0
    else:
        return 1


if __name__ == "__main__":
    sys.exit(main())