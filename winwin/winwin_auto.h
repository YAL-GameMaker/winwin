#pragma once

/**
    @dllg:type ww_ptr_create
    @gmlRead
    var _ptr = buffer_read(_buf, buffer_u64);
    var _box;
    if (_ptr != 0) {
        _ptr = ptr(_ptr);
        _box = new winwin(_ptr);
        winwin_map[?_ptr] = _box;
        ds_list_add(winwin_list, _box);
    } else _box = undefined;
    return _box;
**/
/**
    @dllg:type ww_ptr_find
    @gmlRead
    var _ptr = buffer_read(_buf, buffer_u64);
    var _box;
    if (_ptr != 0) {
        _ptr = ptr(_ptr);
        _box = global.__winwin_map[?_ptr];
        if (_box == undefined) {
            _box = new winwin(_ptr);
            winwin_map[?_ptr] = _box;
            ds_list_add(winwin_list, _box);
        }
    } else _box = undefined;
    return _box;
**/
/**
    @dllg:type ww_ptr_destroy
    @gmlWrite
    var _box_0 = $value;
    if (instanceof(_box_0) != "winwin") { show_error("Expected a winwin, got " + string(_box_0), true); exit }
    var _ptr_0 = _box_0.__ptr__;
    if (_ptr_0 == pointer_null) { show_error("This winwin is already destroyed.", true); exit; }
    _box_0.__ptr__ = pointer_null;
    ds_map_delete(winwin_map, _ptr_0);
    var _ind = ds_list_find_index(winwin_list, _box_0);
    if (_ind >= 0) ds_list_delete(winwin_list, _ind);
    buffer_write(_buf, buffer_u64, int64(_ptr_0));
**/