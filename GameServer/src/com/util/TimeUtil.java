package com.util;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.TimeZone;

import org.apache.log4j.Logger;
import org.apache.oro.text.regex.MatchResult;
import org.apache.oro.text.regex.Pattern;
import org.apache.oro.text.regex.PatternCompiler;
import org.apache.oro.text.regex.PatternMatcher;
import org.apache.oro.text.regex.PatternMatcherInput;
import org.apache.oro.text.regex.Perl5Compiler;
import org.apache.oro.text.regex.Perl5Matcher;

public class TimeUtil {

	private static Logger log = Logger.getLogger(TimeUtil.class);

	private static String reg = "\\[([\\d|\\*]+)\\]\\[([\\d|\\*]+)\\](\\[([\\d|\\*|\\-|\\,]+)\\])?(\\[([w|W|\\d|\\-|\\,|\\*]+)\\])?(\\[(\\d+):(\\d+)-(\\d+):(\\d+)\\])";

	// private static SimpleDateFormat sdf = new
	// SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	/**
	 * ת��ʱ���ʽ
	 * 
	 * @param time
	 *            ���������ʽʱ���ַ���
	 * @return
	 */
	public static List<String> translate(String time) {
		List<String> times = new ArrayList<String>();
		try {
			PatternCompiler complier = new Perl5Compiler();
			PatternMatcher matcher = new Perl5Matcher();
			Pattern patternForLink = complier.compile(reg,
					Perl5Compiler.CASE_INSENSITIVE_MASK);

			PatternMatcherInput input = new PatternMatcherInput(time);
			if (matcher.matches(input, patternForLink)) {
				MatchResult match = matcher.getMatch();

				StringBuffer buf = new StringBuffer();
				// ��
				buf.append("0 ");
				// ��
				buf.append(match.group(9)).append(" ");
				// Сʱ
				buf.append(match.group(8)).append(" ");
				// ��
				if (match.group(4) != null) {
					if (("*").equals(match.group(4)) && match.group(6) != null
							&& !("*").equals(match.group(6))) {
						buf.append("?").append(" ");
					} else {
						buf.append(match.group(4)).append(" ");
					}
				} else {
					if (match.group(6) != null && !("*").equals(match.group(6))) {
						buf.append("?").append(" ");
					} else {
						buf.append("*").append(" ");
					}
				}
				// ��
				buf.append(match.group(2)).append(" ");
				// ����
				if (match.group(6) != null) {
					if (("*").equals(match.group(6)) && match.group(4) != null) {
						buf.append("?").append(" ");
					} else {
						buf.append(replaceMonth(match.group(6))).append(" ");
					}
				} else {
					if (match.group(4) != null) {
						buf.append("?").append(" ");
					} else {
						buf.append("*").append(" ");
					}
				}
				// ��
				buf.append(match.group(1)).append(" ");
				// ʱ�俪ʼʱ��
				times.add(buf.toString());

				buf = new StringBuffer();
				// ��
				buf.append("59 ");
				// ��
				buf.append(match.group(11)).append(" ");
				// Сʱ
				buf.append(match.group(10)).append(" ");
				// ��
				if (match.group(4) != null) {
					if (("*").equals(match.group(4)) && match.group(6) != null
							&& !("*").equals(match.group(6))) {
						buf.append("?").append(" ");
					} else {
						buf.append(match.group(4)).append(" ");
					}
				} else {
					if (match.group(6) != null && !("*").equals(match.group(6))) {
						buf.append("?").append(" ");
					} else {
						buf.append("*").append(" ");
					}
				}
				// ��
				buf.append(match.group(2)).append(" ");
				// ����
				if (match.group(6) != null) {
					if (("*").equals(match.group(6)) && match.group(4) != null) {
						buf.append("?").append(" ");
					} else {
						buf.append(replaceMonth(match.group(6))).append(" ");
					}
				} else {
					if (match.group(4) != null) {
						buf.append("?").append(" ");
					} else {
						buf.append("*").append(" ");
					}
				}
				// ��
				buf.append(match.group(1)).append(" ");
				// ʱ�俪ʼʱ��
				times.add(buf.toString());
			}
		} catch (Exception e) {
			log.error(e, e);
		}
		return times;
	}

	public static Date getAfterDayTime(Date d, int day) {
		Calendar calendar = new GregorianCalendar();
		calendar.setTime(d);
		calendar.add(Calendar.DATE, day);
		return calendar.getTime();
	}

	/**
	 * �����滻
	 * 
	 * @param week
	 * @return
	 */
	private static String replaceMonth(String week) {
		// ����һ MON ���ڶ� TUE ������ WED ������ THU ������ FRI ������ SAT ������ SUN
		return week.replaceAll("w1", "MON").replaceAll("w2", "TUE")
				.replaceAll("w3", "WED").replaceAll("w4", "THU")
				.replaceAll("w5", "FRI").replaceAll("w6", "SAT")
				.replaceAll("w7", "SUN");
	}

	/**
	 * ����ʱ���Ƿ���ָ��ʱ�����
	 * 
	 * @param time
	 * @param times
	 * @return
	 */
	public static boolean isSatisfiedBy(String time, long times) {
		return isSatisfiedBy(time, new Date(times));
	}

	/**
	 * ����ʱ���Ƿ���ָ��ʱ�����
	 * 
	 * @param time
	 * @param date
	 * @return
	 */
	@SuppressWarnings("deprecation")
	public static boolean isSatisfiedBy(String time, Date date) {

		try {
			PatternCompiler complier = new Perl5Compiler();
			PatternMatcher matcher = new Perl5Matcher();
			Pattern patternForLink = complier.compile(reg,
					Perl5Compiler.CASE_INSENSITIVE_MASK);

			String[] times = time.split(Symbol.FENHAO_REG);
			for (int i = 0; i < times.length; i++) {
				PatternMatcherInput input = new PatternMatcherInput(times[i]);
				if (matcher.matches(input, patternForLink)) {
					MatchResult match = matcher.getMatch();

					StringBuffer buf = new StringBuffer();
					// ��
					buf.append("* ");
					// ��
					buf.append("* ");
					// Сʱ
					buf.append("* ");
					// ��
					if (match.group(4) != null) {
						if (("*").equals(match.group(4))
								&& match.group(6) != null
								&& !("*").equals(match.group(6))) {
							buf.append("?").append(" ");
						} else {
							buf.append(match.group(4)).append(" ");
						}
					} else {
						if (match.group(6) != null
								&& !("*").equals(match.group(6))) {
							buf.append("?").append(" ");
						} else {
							buf.append("*").append(" ");
						}
					}
					// ��
					buf.append(match.group(2)).append(" ");
					// ����
					if (match.group(6) != null) {
						if (("*").equals(match.group(6))
								&& match.group(4) != null) {
							buf.append("?").append(" ");
						} else {
							buf.append(replaceMonth(match.group(6)))
									.append(" ");
						}
					} else {
						if (match.group(4) != null) {
							buf.append("?").append(" ");
						} else {
							buf.append("*").append(" ");
						}
					}
					// ��
					buf.append(match.group(1)).append(" ");

					// CronExpression exp = new CronExpression(buf.toString());
					// �꣬�£��գ����ڼ��ɹ�
					// if (exp.isSatisfiedBy(date)) {
					// ��ʼʱ��
					Calendar begin = Calendar.getInstance();
					begin.set(Calendar.HOUR_OF_DAY,
							Integer.parseInt(match.group(8)));
					begin.set(Calendar.MINUTE, Integer.parseInt(match.group(9)));
					begin.set(Calendar.SECOND, 0);
					begin.add(Calendar.SECOND, -1);
					// ����ʱ��
					Calendar end = Calendar.getInstance();
					end.set(Calendar.HOUR_OF_DAY,
							Integer.parseInt(match.group(10)));
					end.set(Calendar.MINUTE, Integer.parseInt(match.group(11)));
					end.set(Calendar.SECOND, 59);
					end.add(Calendar.SECOND, 1);

					Calendar cal = Calendar.getInstance();
					cal.set(Calendar.HOUR_OF_DAY, date.getHours());
					cal.set(Calendar.MINUTE, date.getMinutes());
					cal.set(Calendar.SECOND, date.getSeconds());
					// �ж�ʱ���Ƿ����
					if (cal.after(begin) && cal.before(end))
						return true;
					// }
				}
			}
		} catch (Exception e) {
			log.error(e, e);
		}

		return false;
	}

	/**
	 * ���������Ƿ���ָ��ʱ�����
	 * 
	 * @param time
	 * @return
	 */
	public static boolean isNowSatisfiedBy(String time) {
		return isSatisfiedBy(time, System.currentTimeMillis());
	}

	/**
	 * ��ȡָ��ʱ�䵽���ڵ�ʱ���������룩
	 * 
	 * @param time
	 * @return
	 */
	public static long getDurationToNow(long time) {
		return System.currentTimeMillis() - time;
	}

	/**
	 * ��ȡָ��ʱ�䵽���ڵ�ʱ�������룩
	 * 
	 * @param time
	 * @return ��
	 */
	public static int getDurationToNowSec(long time) {
		return (int) (getDurationToNow(time) / 1000);
	}

	/**
	 * �ж�����ʱ���Ƿ���ͬһ��
	 * 
	 * @param time
	 * @param time2
	 * @return
	 */
	public static boolean isSameDay(long time, long time2) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		int i = instance.get(Calendar.DAY_OF_YEAR);
		instance.setTimeInMillis(time2);
		int j = instance.get(Calendar.DAY_OF_YEAR);
		return i == j;
	}

	/**
	 * �ж��Ƿ��ڽ��켸��֮�� trueΪ����ָ��ʱ��
	 * 
	 * @param hour
	 * @return
	 */
	public static boolean isAfterHourOfCurrentDay(int hour, long time) {
		long currentTimeMillis = System.currentTimeMillis();
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(currentTimeMillis);
		instance.set(Calendar.HOUR_OF_DAY, hour);
		instance.set(Calendar.MINUTE, 0);
		instance.set(Calendar.SECOND, 0);
		long timeInMillis = instance.getTimeInMillis();
		return time - timeInMillis > 0;
	}

	/**
	 * ָ��ʱ������
	 * 
	 * @param time
	 * @return
	 */
	public static int getYear(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		return instance.get(Calendar.YEAR);
	}

	/**
	 * ָ��ʱ����·�
	 * 
	 * @param time
	 * @return
	 */
	public static int getMonth(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		return instance.get(Calendar.MONTH);
	}

	/**
	 * ��ȡ����
	 * 
	 * @param time
	 * @return
	 */
	public static int getDayOfMonth(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		return instance.get(Calendar.DAY_OF_MONTH);
	}

	/**
	 * ��ȡָ��ʱ�� ��һ���ڵĵڼ���
	 * 
	 * @param time
	 * @return
	 */
	public static int getDayOfWeekInMonth(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		return instance.get(Calendar.DAY_OF_WEEK_IN_MONTH);
	}

	/**
	 * ��ȡ���ڼ�
	 * 
	 * @param time
	 * @return
	 */
	public static int getDayOfWeek(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		int i = instance.get(Calendar.DAY_OF_WEEK);
		if (i == 1) {
			return 7;
		} else {
			i -= 1;
		}
		return i;
	}

	/**
	 * ��ȡһ���ڵĵڼ���
	 * 
	 * @param time
	 * @return
	 */
	public static int getDayOfYear(long time) {
		Calendar instance = Calendar.getInstance();
		instance.setTimeInMillis(time);
		return instance.get(Calendar.DAY_OF_YEAR);
	}

	/**
	 * ��ȡ���ʱ��
	 * 
	 * @param time
	 * @return
	 */
	public static Date getNextValidTime(String time) {
		try {
			String[] times = time.split(Symbol.FENHAO_REG);
			Date date = null;
			for (int i = 0; i < times.length; i++) {
				//List<String> list = TimeUtil.translate(time);
				Date temp = new Date();
				if (date == null || temp.before(date))
					date = temp;
			}
			return date;
		} catch (Exception e) {
			log.error(e);
		}
		return null;
	}

	/**
	 * ��ȡ1970��������� ������������ÿ��ָ����Сʱ+1�������ж�ÿ��X��������֮��ģ�
	 * 
	 * @param hour
	 *            ÿ���X��Сʱ+1
	 * @return
	 */
	public static int GetCurDay(int hour) {
		TimeZone zone = TimeZone.getDefault(); // Ĭ��ʱ��
		long s = System.currentTimeMillis() / 1000 + hour * 3600;
		if (zone.getRawOffset() != 0) {
			s = s + zone.getRawOffset() / 1000;
		}
		s = s / 86400;
		return (int) s;
	}

	/**
	 * ��ȡ1970�����ʱ��, 1��ȡ�룬2 ���ӣ�3Сʱ��4����,5����
	 * 
	 * @param x
	 * @return
	 */
	public static long GetCurTimeInMin(int x) {
		TimeZone zone = TimeZone.getDefault(); // Ĭ��ʱ��
		long s = System.currentTimeMillis() / 1000;
		if (zone.getRawOffset() != 0) {
			s = s + zone.getRawOffset() / 1000;
		}
		switch (x) {
		case 1:
			break;
		case 2:
			s = s / 60;
			break;
		case 3:
			s = s / 3600;
			break;
		case 4:
			s = s / 86400;
			break;
		case 5:
			s = s / 86400 + 3;// ��������������1��7��һ��
			s = s / 7;
			break;
		default:
			break;
		}
		return s;
	}

	/**
	 * ��ȡ����ָ��ʱ���UNIXʱ��
	 * 
	 * @param hour
	 * @param minute
	 * @param second
	 * @param millisecond
	 * @return
	 */
	public static long getTheDayUnixTime(int hour, int minute, int second,
			int millisecond) {
		Calendar cal = Calendar.getInstance();
		cal.set(Calendar.HOUR, hour);
		cal.set(Calendar.MINUTE, minute);
		cal.set(Calendar.SECOND, second);
		cal.set(Calendar.MILLISECOND, millisecond);
		return cal.getTimeInMillis();
	}

	public static void main(String[] args) {
		// long time=System.currentTimeMillis();
		// System.out.println(getDayOfMonth(time));
		// System.out.println(getDayOfWeek(time));
		// System.out.println(getDayOfYear(time));
		// System.out.println(getDayOfWeekInMonth(time));
		// System.out.println(getMonth(time));
		// System.out.println(getYear(time));
		System.out.println(TimeUtil.isSatisfiedBy(
				"[*][*][*][*][13:05-18:05];[*][*][*][*][20:05-20:05] ",
				System.currentTimeMillis()));

		//
		//
		// List<String> list =
		// TimeUtil.translate("[2011][11][w1,w2,w5][18:05-18:06]");
		// list.addAll(TimeUtil.translate("[*][*][10-20][18:05-18:05]"));
		// for (int i = 0; i < list.size(); i++) {
		// System.out.println(list.get(i));
		// try{
		// CronExpression exp = new CronExpression("0 05 18 * * ? * ");
		// System.out.println(exp.getNextValidTimeAfter(new Date(time)));
		// }catch (Exception e) {
		// e.printStackTrace();
		// }
		// }
		// long time=System.currentTimeMillis()-1000*60*60*24;
		// System.out.println(isAfterHourOfCurrentDay(5, time));

	}

	/**
	 * ��ȡ��ʽ����ʱ���ַ���
	 * 
	 * @param date
	 * @return
	 */
	public static String getStringDate(Date date) {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(
				"yyyy-MM-dd HH:mm:ss");
		return simpleDateFormat.format(date);
	}

	/**
	 * ��ȡ��ǰ��ʽ����ʱ���ַ���
	 * 
	 * @param date
	 * @return
	 */
	public static String getNowStringDate() {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(
				"yyyy-MM-dd HH:mm:ss");
		return simpleDateFormat.format(new Date());
	}

	public static Date getDateByString(String date) {
		try {
			SimpleDateFormat simpleDateFormat = new SimpleDateFormat(
					"yyyy-MM-dd HH:mm:ss");
			return simpleDateFormat.parse(date);
		} catch (ParseException e) {
			e.printStackTrace();
			log.error("{}���ڸ�ʽ����{}" + date + "yyyy-MM-dd HH:mm:ss");
			return null;
		}
	}

	/**
	 * �ж��ǲ��ǽ���
	 * 
	 * @author zhouyu
	 * @dateTime 2011-11-14 ����09:38:55
	 * @param time
	 *            ʱ����봮(<strong>ע��:��ȷ������</strong>)
	 * @return
	 */
	public static boolean isToday(final Long time) {
		if (null == time) {
			return false;
		}
		Calendar target = Calendar.getInstance();
		Calendar today = Calendar.getInstance();
		target.setTimeInMillis(time);
		if (target.get(Calendar.YEAR) == today.get(Calendar.YEAR)
				&& target.get(Calendar.MONTH) == today.get(Calendar.MONTH)
				&& target.get(Calendar.DAY_OF_MONTH) == today
						.get(Calendar.DAY_OF_MONTH)) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * �ǲ��Ǳ���
	 * 
	 * @author zhouyu
	 * @dateTime 2011-11-16 ����06:05:27
	 * @param time
	 *            ʱ����봮 <string>ע�⣺��ȷ������</strong>
	 * @return
	 */
	public static boolean isCurrentWeek(final Long time) {
		if (null == time) {
			return false;
		}
		Calendar target = Calendar.getInstance();
		Calendar today = Calendar.getInstance();
		target.setTimeInMillis(time);
		if (target.get(Calendar.YEAR) == today.get(Calendar.YEAR)
				&& target.get(Calendar.WEEK_OF_YEAR) == today
						.get(Calendar.WEEK_OF_YEAR)) {
			return true;
		} else {
			return false;
		}
	}

}
