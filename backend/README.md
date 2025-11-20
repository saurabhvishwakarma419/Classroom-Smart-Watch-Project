# Classroom Smart  Watch - Backend API

## Overview
This is the backend server for the Smart Classroom Watch system. It provides RESTful APIs for mobile apps and web dashboards, handles real-time data processing, and manages the analytics engine.

## Tech Stack
- **Runtime:** Node.js v14+
- **Framework:** Express.js
- **Database:** MongoDB / PostgreSQL
- **Real-time:** Socket.io / MQTT
- **Analytics:** Python 3.8+
- **Authentication:** JWT
- **Documentation:** Swagger/OpenAPI

## Prerequisites
- Node.js v14 or higher
- MongoDB v4.4+ or PostgreSQL v12+
- Python 3.8+ (for analytics)
- Redis (optional, for caching)

## Installation

### 1. Clone and Install Dependencies
```bash
cd backend/
npm install
pip install -r requirements.txt
```

### 2. Environment Setup
Create a `.env` file in the backend directory:

NODE_ENV=development
PORT=5000
DATABASE_URL=mongodb://localhost:27017/smart_classroom
JWT_SECRET=your_jwt_secret_key_here
MQTT_BROKER=mqtt://localhost:1883
REDIS_URL=redis://localhost:6379
### 3. Database Setup
```bash
# For MongoDB
npm run db:migrate

# For PostgreSQL
psql -U postgres -d smart_classroom -f database/schema.sql
```

### 4. Start Server
```bash
# Development mode
npm run dev

# Production mode
npm start
```

## API Endpoints

### Authentication
- `POST /api/auth/login` - User login
- `POST /api/auth/register` - User registration
- `POST /api/auth/refresh` - Refresh JWT token

### Attendance
- `GET /api/attendance/class/:classId` - Get class attendance
- `POST /api/attendance/mark` - Mark attendance (from watch)
- `GET /api/attendance/student/:studentId` - Get student attendance history

### Students
- `GET /api/students` - Get all students
- `GET /api/students/:id` - Get student by ID
- `POST /api/students` - Create new student
- `PUT /api/students/:id` - Update student
- `DELETE /api/students/:id` - Delete student

### Focus Analytics
- `GET /api/analytics/focus/:studentId` - Get student focus data
- `GET /api/analytics/class/:classId` - Get class engagement metrics
- `POST /api/analytics/process` - Process raw sensor data

### Quizzes
- `POST /api/quizzes/create` - Create quiz
- `POST /api/quizzes/submit` - Submit quiz response
- `GET /api/quizzes/:quizId/results` - Get quiz results

### Emergency
- `POST /api/emergency/alert` - Trigger SOS alert
- `GET /api/emergency/history` - Get emergency history

## Testing
```bash
# Run all tests
npm test

# Run with coverage
npm run test:coverage
```

## Deployment
```bash
# Build for production
npm run build

# Start production server
npm run start:prod
```

## Contributing
Please read CONTRIBUTING.md for details on our code of conduct and the process for submitting pull requests.

## License
MIT License - see LICENSE file for details
