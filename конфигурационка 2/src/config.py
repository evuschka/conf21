"""
Модуль для работы с конфигурацией приложения.
Содержит класс Configuration для хранения и валидации параметров.
"""

import os
import re
from urllib.parse import urlparse
from typing import Optional, Dict, Any
from .errors import PackageNameError, RepositoryError, TestModeError, ValidationError # type: ignore


class Configuration:
    """
    Класс для хранения и валидации конфигурации приложения.
    
    Атрибуты:
        package_name (str): Имя анализируемого пакета
        repository (str): URL репозитория или путь к файлу
        test_repo_mode (bool): Режим работы с тестовым репозиторием
        is_url (bool): Флаг, указывающий, что repository является URL
    """
    
    def __init__(self, package_name: str, repository: str, test_repo_mode: str = "off"):
        """
        Инициализация конфигурации.
        
        Args:
            package_name: Имя анализируемого пакета
            repository: URL репозитория или путь к файлу
            test_repo_mode: Режим работы с тестовым репозиторием ('on'/'off')
            
        Raises:
            ConfigurationError: При ошибках валидации параметров
        """
        self.package_name = package_name
        self.repository = repository
        self.test_repo_mode = test_repo_mode
        self.is_url = False
        
        self._validate_all()
    
    def _validate_package_name(self) -> None:
        """Валидация имени пакета."""
        if not self.package_name:
            raise PackageNameError("Имя пакета не может быть пустым")
        
        if not isinstance(self.package_name, str):
            raise PackageNameError("Имя пакета должно быть строкой")
        
        # Проверка на допустимые символы в имени пакета
        if not re.match(r'^[a-zA-Z0-9_\-\.]+$', self.package_name):
            raise PackageNameError(
                f"Недопустимое имя пакета: {self.package_name}. "
                "Допустимы только буквы, цифры, точки, дефисы и подчеркивания"
            )
        
        if len(self.package_name) > 100:
            raise PackageNameError("Имя пакета слишком длинное (максимум 100 символов)")
    
    def _validate_repository(self) -> None:
        """Валидация репозитория (URL или пути к файлу)."""
        if not self.repository:
            raise RepositoryError("Репозиторий не может быть пустым")
        
        if not isinstance(self.repository, str):
            raise RepositoryError("Репозиторий должен быть указан строкой")
        
        # Проверяем, является ли значение URL
        try:
            result = urlparse(self.repository)
            self.is_url = all([result.scheme, result.netloc])
            
            if self.is_url:
                # Проверяем допустимые схемы для URL
                if result.scheme not in ['http', 'https', 'ftp', 'file']:
                    raise RepositoryError(
                        f"Недопустимая схема URL: {result.scheme}. "
                        "Допустимы: http, https, ftp, file"
                    )
            else:
                # Если не URL, проверяем как путь к файлу
                if not os.path.exists(self.repository):
                    raise RepositoryError(
                        f"Файл не существует: {self.repository}"
                    )
                
                if not os.path.isfile(self.repository):
                    raise RepositoryError(
                        f"Указанный путь не является файлом: {self.repository}"
                    )
        except ValueError as e:
            raise RepositoryError(f"Некорректный формат репозитория: {str(e)}")
    
    def _validate_test_repo_mode(self) -> None:
        """Валидация режима работы с тестовым репозиторием."""
        valid_modes = ['on', 'off']
        
        if not isinstance(self.test_repo_mode, str):
            raise TestModeError("Режим работы должен быть строкой")
        
        if self.test_repo_mode.lower() not in valid_modes:
            raise TestModeError(
                f"Недопустимый режим работы: {self.test_repo_mode}. "
                f"Допустимые значения: {', '.join(valid_modes)}"
            )
        
        # Приводим к нижнему регистру для единообразия
        self.test_repo_mode = self.test_repo_mode.lower()
    
    def _validate_all(self) -> None:
        """Выполняет полную валидацию всех параметров."""
        try:
            self._validate_package_name()
            self._validate_repository()
            self._validate_test_repo_mode()
        except (PackageNameError, RepositoryError, TestModeError) as e:
            # Обертываем специфические ошибки в общую ValidationError
            raise ValidationError(f"Ошибка валидации конфигурации: {str(e)}")
    
    def to_dict(self) -> Dict[str, Any]:
        """
        Возвращает конфигурацию в виде словаря.
        
        Returns:
            Словарь с параметрами конфигурации
        """
        return {
            'package_name': self.package_name,
            'repository': self.repository,
            'test_repo_mode': self.test_repo_mode,
            'repository_type': 'URL' if self.is_url else 'файл'
        }
    
    def __str__(self) -> str:
        """Строковое представление конфигурации."""
        config_dict = self.to_dict()
        return "\n".join([f"{key}: {value}" for key, value in config_dict.items()])