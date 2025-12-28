export const keyCodeMap = {
    'KeyA': 0,
    'KeyS': 1,
    'KeyD': 2,
    'KeyF': 3,
    'KeyH': 4,
    'KeyG': 5,
    'KeyZ': 6,
    'KeyX': 7,
    'KeyC': 8,
    'KeyV': 9,
    'Return': 36,
    'Enter': 36,
    'Escape': 53,
    'ArrowLeft': 123,
    'ArrowRight': 124,
    'ArrowDown': 125,
    'ArrowUp': 126,
    'Space': 49,
    'Tab': 48,
    'Backspace': 51,
    'Delete': 117,
    'Digit0': 29,
    'Digit1': 18,
    'Digit2': 19,
    'Digit3': 20,
    'Digit4': 21,
    'Digit5': 23,
    'Digit6': 22,
    'Digit7': 26,
    'Digit8': 28,
    'Digit9': 25
};

export function mapKeyCode(code) {
    return keyCodeMap[code] ?? 53;
}
