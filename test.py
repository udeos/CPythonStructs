# coding: utf-8
import unittest
from structs import OneLinkedList


class TestOneLinkedList(unittest.TestCase):

    def test_init(self):
        self.assertRaises(TypeError, OneLinkedList, 1)

    def test_repr(self):
        self.assertEqual(repr(OneLinkedList()), "OneLinkedList (size: 0) []")
        self.assertEqual(repr(OneLinkedList([1, 2])), "OneLinkedList (size: 2) [1, 2]")

    def test_f_get(self):
        obj = OneLinkedList([1, 2])
        self.assertEqual(obj.get(0), 1)
        self.assertEqual(obj.get(1), 2)
        self.assertEqual(obj.get(3), None)

    def test_f_pop(self):
        obj = OneLinkedList([1])
        self.assertEqual(obj.pop(), 1)
        self.assertEqual(obj.pop(), None)

    def test_f_push(self):
        obj = OneLinkedList([1])
        obj.push(2)
        self.assertEqual(obj.to_list(), [2, 1])

    # def test_f_insert(self):
    #     obj = OneLinkedList([1,3])
    #     obj.insert(1, 2)
    #     self.assertEqual(obj.to_list(), [1, 2, 3])
    #     obj.insert(666, 5)
    #     self.assertEqual(obj.to_list(), [1, 2, 3, 5])
    #     obj.insert(-1, 4)
    #     self.assertEqual(obj.to_list(), [1, 2, 3, 4, 5])

    def test_f_to_list(self):
        self.assertEqual(OneLinkedList().to_list(), [])
        self.assertEqual(OneLinkedList((1, 2)).to_list(), [1, 2])
        self.assertEqual(OneLinkedList([1, 2]).to_list(), [1, 2])

    def test_f_reverse(self):
        self.assertEqual(OneLinkedList([1, 2]).reverse().to_list(), [2, 1])
        self.assertEqual(OneLinkedList().reverse().to_list(), [])

    # def test_magic_len(self):
        # self.assertEqual(len(OneLinkedList()), 0)
        # self.assertEqual(len(OneLinkedList([1, 2])), 2)


if __name__ == '__main__':
    unittest.main()

# итерирование
# мусор
# тесты
# документация
# сравнение за предикатом
# замер производительности
# размер
# delete
# insert
# take
# index
