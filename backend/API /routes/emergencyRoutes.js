const express = require('express');
const router = express.Router();
const emergencyController = require('../controllers/emergencyController');
const { auth, authorize } = require('../middleware/auth');

// @route   POST /api/emergency/alert
// @desc    Trigger SOS alert
// @access  Private (Student)
router.post('/alert', auth, authorize('student'), emergencyController.triggerAlert);

// @route   GET /api/emergency/active
// @desc    Get active alerts
// @access  Private (Teacher, Admin, Security)
router.get('/active', auth, authorize('teacher', 'admin', 'security'), emergencyController.getActiveAlerts);

// @route   PUT /api/emergency/:alertId/acknowledge
// @desc    Acknowledge alert
// @access  Private (Teacher, Admin, Security)
router.put('/:alertId/acknowledge', auth, authorize('teacher', 'admin', 'security'), emergencyController.acknowledgeAlert);

// @route   PUT /api/emergency/:alertId/resolve
// @desc    Resolve alert
// @access  Private (Teacher, Admin, Security)
router.put('/:alertId/resolve', auth, authorize('teacher', 'admin', 'security'), emergencyController.resolveAlert);

// @route   GET /api/emergency/history
// @desc    Get emergency history
// @access  Private (Admin, Security)
router.get('/history', auth, authorize('admin', 'security'), emergencyController.getEmergencyHistory);

module.exports = router;
