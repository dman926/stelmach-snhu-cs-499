import type { JwtPayload } from "jsonwebtoken";

declare global {
  namespace NodeJS {
    interface ProcessEnv {
      JWT_SECRET: string;
    }
  }

  namespace Express {
    interface Request {
      auth: string | JwtPayload | undefined;
    }
  }
}
